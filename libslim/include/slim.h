/*
Copyright (c) 2020, chyyran

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the copyright holder nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL YELLOW WOOD GOBLIN BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __ELM_H__
#define __ELM_H__

#include <nds/disc_io.h>

#ifdef __cplusplus
extern "C"
{
#endif
  /**
   * Initializes the given mount point with the provided DISC_INTERFACE
   * 
   * - `mount` must be either "sd:" or "fat:". Unlike libfat, this function does not
   * support other mount points.
   */
  bool fatMountSimple(const char *mount, const DISC_INTERFACE *interface);

  /**
   * Unmounts the given mount point, attempting to flush any open files.
   * 
   * Unlike libfat where this is a void function, 
   * this function returns true on success.
   */
  bool fatUnmount(const char *mount);

  /**
   * Initializes SD card in sd:/ and SLOT-1 flashcart device in fat:/ with the default
   * DISC_INTERFACE, then sets the default directory to sd:/ if available, or 
   * fat:/ otherwise, and then changes directory according to argv.
   * 
   * Returns true if either sd:/ or fat:/ is successfully mounted, and false otherwise.
   */
  bool fatInitDefault(void);

  /**
   * Initializes SD card in sd:/ and SLOT-1 flashcart device in fat:/ with the default
   * DISC_INTERFACE, then sets the default directory to sd:/ if available, or 
   * fat:/ otherwise, and then changes directory according to argv if setArgvMagic 
   * is true.
   * 
   * This method differs significantly enough from its equivalent libfat API that
   * you should take caution. Particularly of note is that unlike libfat, the 
   * cluster cache is shared across all mounted devices.
   * 
   * Returns true if either sd:/ or fat:/ is successfully mounted, and false otherwise.
   */
  bool fatInit(uint32_t cacheSize, bool configureDefault);

  /**
   * Gets the volume label of the given mount point.
   * 
   * - `mount` must be either "sd:" or "fat:". Unlike libfat, this function does not
   * support other mount points.
   */
  void fatGetVolumeLabel(const char *mount, char *label);

  /**
   * Gets the given FAT attributes for the file.
   */
  int FAT_getAttr(const char *file);

  /**
   * Sets the FAT attributes
   */
  int FAT_setAttr(const char *file, uint8_t attr);

  /**
   * Configures the default device after mounting is successful.
   * 
   * root should either be `sd:/` or `fat:/`. Any other path is undefined behaviour.
   */
  bool configureDefault(const char *root);

  /**
   * Configures the size of the global cache size. 
   * Setting a cache size of 0 will disable the cache forever.
   * 
   * Once configured, further calls of this method will have no effect.
   * 
   * This must be called before calling any method that initializes or
   * mounts a FAT device. Otherwise, the cache will automatically be initialized
   * with size SLIM_CACHE_SIZE (64 by default).
   * 
   */
  bool configureCache(uint32_t cacheSize);

  /**
   * Get the 8.3 short file name (SFN) of a given UTF-8 file path.
   */
  void SLIM_getsfn(const char *_path,char *sfn);

// File attributes
#define ATTR_ARCHIVE    0x20   // Archive
#define ATTR_DIRECTORY  0x10 // Directory
#define ATTR_VOLUME     0x08    // Volume
#define ATTR_SYSTEM     0x04    // System
#define ATTR_HIDDEN     0x02    // Hidden
#define ATTR_READONLY   0x01  // Read only

#ifdef __cplusplus
}
#endif

#define MAX_FILENAME_LENGTH 768 // 256 UCS-2 characters encoded into UTF-8 can use up to 768 UTF-8 chars

#endif