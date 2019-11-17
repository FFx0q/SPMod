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
    class ISQliteColumn;

    class ISQLiteStatement
    {
    public:
        /**
         * @brief Check if query has result.
         *
         * @param errormsg String where error string will be stored.
         * @param size
         *
         * @return True if has, false otherwise.
         */
        virtual bool hasResult(char *errormsg, std::size_t size) = 0;

        /**
         * @brief Retrieves result.
         *
         * @return SQLiteColumn Pointer.
         */
        ISQliteColumn *getResult() = 0;

        /**
         * @brief Binds a value to a specify parametr.
         *
         * @param index
         * @param value
         */
        virtual void bind(int index, const int value) = 0;

        /**
         * @brief Binds a value to a specify parametr.
         *
         * @param index
         * @param value
         */
        virtual void bind(int index, const float value) = 0;
        /**
         * @brief Binds a value to a specify parametr.
         *
         * @param index
         * @param value
         */
        virtual void bind(int index, const char *value) = 0;

        /**
         * @brief Binds a value to a specify parametr.
         *
         * @param index
         * @param value
         * @param size
         */
        virtual void bind(int index, const void *value, std::size_t size) = 0;

    protected:
        virtual ~ISQLiteStatement() = default;
    };
} // namespace SPMod