# Copyright 2013 Cloudbase Solutions Srl
# All Rights Reserved.
#
#    Licensed under the Apache License, Version 2.0 (the "License"); you may
#    not use this file except in compliance with the License. You may obtain
#    a copy of the License at
#
#         http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
#    WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
#    License for the specific language governing permissions and limitations
#    under the License.
"""
Image caching and management.
"""
import os

from os_win import utilsfactory
from oslo_log import log as logging
from oslo_utils import excutils
from oslo_utils import units

import nova.conf
from nova import exception
from nova.i18n import _
from nova import utils
from nova.virt.hyperv import pathutils
from nova.virt import images

LOG = logging.getLogger(__name__)

CONF = nova.conf.CONF


class ImageCache(object):
    def __init__(self):
        self._pathutils = pathutils.PathUtils()
        self._vhdutils = utilsfactory.get_vhdutils()

    def _get_root_vhd_size_gb(self, instance):
        if instance.old_flavor:
            return instance.old_flavor.root_gb
        else:
            return instance.root_gb

    def _resize_and_cache_vhd(self, instance, vhd_path):
        vhd_size = self._vhdutils.get_vhd_size(vhd_path)['VirtualSize']

        root_vhd_size_gb = self._get_root_vhd_size_gb(instance)
        root_vhd_size = root_vhd_size_gb * units.Gi

        root_vhd_internal_size = (
                self._vhdutils.get_internal_vhd_size_by_file_size(
                    vhd_path, root_vhd_size))

        if root_vhd_internal_size < vhd_size:
            raise exception.FlavorDiskSmallerThanImage(
                flavor_size=root_vhd_size, image_size=vhd_size)
        if root_vhd_internal_size > vhd_size:
            path_parts = os.path.splitext(vhd_path)
            resized_vhd_path = '%s_%s%s' % (path_parts[0],
                                            root_vhd_size_gb,
                                            path_parts[1])

            @utils.synchronized(resized_vhd_path)
            def copy_and_resize_vhd():
                if not self._pathutils.exists(resized_vhd_path):
                    try:
                        LOG.debug("Copying VHD %(vhd_path)s to "
                                  "%(resized_vhd_path)s",
                                  {'vhd_path': vhd_path,
                                   'resized_vhd_path': resized_vhd_path})
                        self._pathutils.copyfile(vhd_path, resized_vhd_path)
                        LOG.debug("Resizing VHD %(resized_vhd_path)s to new "
                                  "size %(root_vhd_size)s",
                                  {'resized_vhd_path': resized_vhd_path,
                                   'root_vhd_size': root_vhd_size})
                        self._vhdutils.resize_vhd(resized_vhd_path,
                                                  root_vhd_internal_size,
                                                  is_file_max_size=False)
                    except Exception:
                        with excutils.save_and_reraise_exception():
                            if self._pathutils.exists(resized_vhd_path):
                                self._pathutils.remove(resized_vhd_path)

            copy_and_resize_vhd()
            return resized_vhd_path

    def get_cached_image(self, context, instance, rescue_image_id=None):
        image_id = rescue_image_id or instance.image_ref

        base_vhd_dir = self._pathutils.get_base_vhd_dir()
        base_vhd_path = os.path.join(base_vhd_dir, image_id)

        @utils.synchronized(base_vhd_path)
        def fetch_image_if_not_existing():
            vhd_path = None
            for format_ext in ['vhd', 'vhdx']:
                test_path = base_vhd_path + '.' + format_ext
                if self._pathutils.exists(test_path):
                    vhd_path = test_path
                    break

            if not vhd_path:
                try:
                    images.fetch(context, image_id, base_vhd_path)

                    format_ext = self._vhdutils.get_vhd_format(base_vhd_path)
                    vhd_path = base_vhd_path + '.' + format_ext.lower()
                    self._pathutils.rename(base_vhd_path, vhd_path)
                except Exception:
                    with excutils.save_and_reraise_exception():
                        if self._pathutils.exists(base_vhd_path):
                            self._pathutils.remove(base_vhd_path)

            return vhd_path

        vhd_path = fetch_image_if_not_existing()

        # Note: rescue images are not resized.
        is_vhd = vhd_path.split('.')[-1].lower() == 'vhd'
        if CONF.use_cow_images and is_vhd and not rescue_image_id:
            # Resize the base VHD image as it's not possible to resize a
            # differencing VHD. This does not apply to VHDX images.
            resized_vhd_path = self._resize_and_cache_vhd(instance, vhd_path)
            if resized_vhd_path:
                return resized_vhd_path

        if rescue_image_id:
            self._verify_rescue_image(instance, rescue_image_id,
                                      vhd_path)

        return vhd_path

    def _verify_rescue_image(self, instance, rescue_image_id,
                             rescue_image_path):
        rescue_image_info = self._vhdutils.get_vhd_info(rescue_image_path)
        rescue_image_size = rescue_image_info['VirtualSize']
        flavor_disk_size = instance.root_gb * units.Gi

        if rescue_image_size > flavor_disk_size:
            err_msg = _('Using a rescue image bigger than the instance '
                        'flavor disk size is not allowed. '
                        'Rescue image size: %(rescue_image_size)s. '
                        'Flavor disk size:%(flavor_disk_size)s.') % dict(
                            rescue_image_size=rescue_image_size,
                            flavor_disk_size=flavor_disk_size)
            raise exception.ImageUnacceptable(reason=err_msg,
                                              image_id=rescue_image_id)
