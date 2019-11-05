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

#include "ext.hpp"

namespace SPSQLiteModule
{
    class SQLiteModuleInterface : public SPMod::IModuleInterface
    {
        public:
            SQLiteModuleInterface();

            const char *getName() const override 
            {
                return "Sqlite";
            }

            std::uint32_t getVersion() const override
            {
                return 1U;
            }

            const char *getAuthor() const override
            {
                return "SPMod Development team";
            }

            const char *getUrl() const override
            {
                return "https://www.github.com/Amaroq7/SPMod";
            }

            const char *getExtName() const override
            {
                return "Sqlite Module";
            }

            void *getImplementation() const override
            {
                return m_impl.get(); 
            }
        private:
            std::unique_ptr<SQLiteInterface> m_impl;
    };
}