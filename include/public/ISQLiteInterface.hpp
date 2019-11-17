/*
 *  Copyright (C) 2018 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <cstddef>

namespace SPMod
{
    class ISQLiteHandler;
    class ISQLiteIterface
    {
    public:
        /**
         * @brief Opens a database connection.
         *
         * @param filename Filename
         * @param errormsg String where error string will be stored.
         * @param size
         *
         * @return SQLiteHandler pointer.
         */
        virtual ISQLiteHandler *connect(const char *filename, char *errormsg, std::size_t size) = 0;
        /**
         * @brief Closes a database connection.
         *
         * @param handle Connection handle.
         *
         * @return True if success, false otherwise.
         */
        virtual bool disconnect(ISQLiteHandler *handle) = 0;

        /**
         * @brief  Check if handle is valid.
         *
         * @param handle Connection handle.
         *
         * @return True if is valid, false otherwise.
         */
        virtual bool isValid(ISQLiteHandler *handle) = 0;

    protected:
        virtual ~ISQLiteIterface() = default;
    };
} // namespace SPMod