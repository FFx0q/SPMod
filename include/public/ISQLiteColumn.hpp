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

namespace SPMod
{
    class ISQLiteColumn
    {
    public:
        /**
         * @brief Returns table name.
         *
         * @return const char*.
         */
        virtual const char *getTableName() = 0;

        /**
         * @brief Get the Int object
         *
         * @return int
         */
        virtual int getInt() = 0;

        /**
         * @brief Get the Float object
         *
         * @return float
         */
        virtual float getFloat() = 0;

        /**
         * @brief Get the Text object
         *
         * @return const char*
         */
        virtual const char *getText() = 0;

        /**
         * @brief Get the Blob object
         *
         * @return const void*
         */
        virtual const void *getBlob() = 0;

    protected:
        virtual ISQLiteColumn() = default;
    };
} // namespace SPMod