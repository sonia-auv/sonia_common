/**
 * \file	system_info.h
 * \author	Thibaut Mattio <thibaut.mattio@gmail.com>
 * \date	18/07/2015
 *
 * \copyright Copyright (c) 2015 S.O.N.I.A. All rights reserved.
 *
 * \section LICENSE
 *
 * This file is part of S.O.N.I.A. software.
 *
 * S.O.N.I.A. software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * S.O.N.I.A. software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with S.O.N.I.A. software. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SONIA_COMMON_SYSTEM_SYSTEM_INFO_H_
#define SONIA_COMMON_SYSTEM_SYSTEM_INFO_H_

#include <string>

#include <sonia_common/macros.h>

namespace sonia_common {

/**
 * Represent a unit of data.
 * A Unit can be a multiple of 10^3B, or a block.
 * The size of a block can be obtained by calling block_size();
 */
enum class BitUnit { BLOCK = 0, B, KB, MB, GB, TB };

/**
 * For a given directory and a bit unit, this method will return the
 * total space of the disk the given path is mounted on.
 *
 * \param unit Change it in order to get the output with a specific unit.
 * \param path A directory being contained by the mounted point to analyze.
 * \return The size of the mounted point where path is located in unit
 */
double TotalPhysicalMemory(BitUnit unit = BitUnit::KB,
                           const char *path = ".") ATLAS_NOEXCEPT;

/**
 * For a given directory and a bit unit, this method will return the
 * free space of the disk the given path is mounted on.
 *
 * The free space is the space available of the root user, not to be confunded
 * with the available space which is the space for other users.
 *
 * \param unit Change it in order to get the output with a specific unit.
 * \param path A directory being contained by the mounted point to analyze.
 * \return The free space on the mounted point where path is located in unit
 */
double FreePhysicalMemory(BitUnit unit = BitUnit::KB,
                          const char *path = ".") ATLAS_NOEXCEPT;

/**
 * For a given directory and a bit unit, this method will return the
 * total space of the disk the given path is mounted on.
 *
 * The available space is the space available for all users, not to be confunded
 * with the free space which is the space for the root user.
 *
 * \param unit Change it in order to get the output with a specific unit.
 * \param path A directory being contained by the mounted point to analyze.
 * \return The available space on the mounted point where path is located in
 *unit
 */
double AvailablePhysicalMemory(BitUnit unit = BitUnit::KB,
                               const char *path = ".") ATLAS_NOEXCEPT;

/**
 * For a given directory and a bit unit, this method will return the
 * used physical memory.
 *
 * It is using both the free space and the available space in order to
 * get an accurate used space.
 *
 * \param unit Change it in order to get the output with a specific unit.
 * \param path A directory being contained by the mounted point to analyze.
 * \return The size of the mounted point where path is located in unit
 */
double UsedPhysicalMemory(BitUnit unit = BitUnit::KB,
                          const char *path = ".") ATLAS_NOEXCEPT;

/**
 * Compare the total space of the mounted point with the used physical memory
 * to get a percentage of used physical memory.
 *
 * \param path A directory being contained by the mounted point to analyze.
 * \return The used space of the mounted point in percentage
 */
double PercentageUsedPhysicalMemory(const char *path = ".") ATLAS_NOEXCEPT;

/**
 * Compare the total space of the mounted point with the used physical memory
 * to get a percentage of available physical memory.
 *
 * \param path A directory being contained by the mounted point to analyze.
 * \return The available space of the mounted point in percentage
 */
double PercentageAvailablePhysicalMemory(const char *path = ".") ATLAS_NOEXCEPT;

/**
 * For the mounted point containing the given path, this will return the size
 * of the block used by the file system.
 *
 * This method is used internally for converting the different units.
 * When calling the statvfs structure, we are getting the size whith a block
 * size unit.
 *
 * \param path A directory being contained by the mounted point to analyze.
 * \return The size of the block for the file system used.
 */
uint64_t BlockSize(const char *path = ".") ATLAS_NOEXCEPT;

/**
 * For the mounted point containing the given path, this will return maximum
 * length of the filenames.
 *
 * \param path A directory being contained by the mounted point to analyze.
 * \return The maximum length of the filenames of the mounted point where path
 * is located in unit
 */
uint64_t MaxFileName(const char *path = ".") ATLAS_NOEXCEPT;

/**
 * Check if the file exist by check it's accessibility
 *
 * \param path of the file we want to check the existance of
 * \return True when the file is accessible (exist)
 */
bool FileExists(const std::string &file_path) ATLAS_NOEXCEPT;

}  // namespace sonia_common

#include <sonia_common/sys/fsinfo_inl.h>

#endif  // SONIA_COMMON_SYSTEM_SYSTEM_INFO_H_
