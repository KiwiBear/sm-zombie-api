/*
 * ============================================================================
 *
 *  Zombie API
 *
 *  File:           boot-modulemanager.inc
 *  Description:    Module manager bootstrapper.
 *
 *  Copyright (C) 2014  Richard Helgeby
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * ============================================================================
 */

/*

USAGE

Use this bootstrapper to properly load and unload your module, and to avoid
duplicated code in multiple modules.

Create the following functions in your plugin. These will be called by this
bootstrapper when the module manager is added or removed.

    ZM_ModuleMgr_OnModuleManagerAdded
    ZM_ModuleMgr_OnModuleManagerRemoved

Call the following functions. The last part of their names tell where you should
call them from:

    ZM_ModuleMgr_AskPluginLoad2
    ZM_ModuleMgr_OnAllPluginsLoaded
    ZM_ModuleMgr_OnPluginEnd
    ZM_ModuleMgr_OnLibraryAdded
    ZM_ModuleMgr_OnLibraryRemoved

*/

/*____________________________________________________________________________*/

new ZM_ModuleManagerLoaded = false;

/*____________________________________________________________________________*/

stock APLRes:ZM_ModuleMgr_AskPluginLoad2(Handle:myself, bool:late, String:error[], err_max)
{
    ZM_SetModuleManagerOptional();
    return APLRes_Success;
}

/*____________________________________________________________________________*/

stock ZM_ModuleMgr_OnAllPluginsLoaded()
{
    if (LibraryExists(LIBRARY_ZM_MODULE_MANAGER))
    {
        ZM_ModuleMgr_OnModuleManagerAdded();
    }
}

/*____________________________________________________________________________*/

stock ZM_ModuleMgr_OnPluginEnd()
{
    ZM_ModuleMgr_OnModuleManagerRemoved();
}

/*____________________________________________________________________________*/

stock ZM_ModuleMgr_OnLibraryAdded(const String:name[])
{
    if (StrEqual(name, LIBRARY_ZM_MODULE_MANAGER))
    {
        ZM_ModuleMgr_OnModuleManagerAdded();
    }
}

/*____________________________________________________________________________*/

stock ZM_ModuleMgr_OnLibraryRemoved(const String:name[])
{
    if (StrEqual(name, LIBRARY_ZM_MODULE_MANAGER))
    {
        ZM_ModuleMgr_OnModuleManagerRemoved();
    }
}

/*____________________________________________________________________________*/

stock ZM_ModuleMgr_OnModuleManagerAdded()
{
    if (ZM_ModuleManagerLoaded)
    {
        return;
    }
    
    ZM_ModuleManagerLoaded = true;
    LogMessage("Module manager available.");
    
    ZM_OnModuleManagerAdded();
}

/*____________________________________________________________________________*/

stock ZM_ModuleMgr_OnModuleManagerRemoved()
{
    if (!ZM_ModuleManagerLoaded)
    {
        return;
    }
    
    ZM_ModuleManagerLoaded = false;
    LogMessage("Module manager removed.");
    
    ZM_OnModuleManagerRemoved();
}

/*____________________________________________________________________________*/

stock ZM_IsModuleManagerLoaded()
{
    return ZM_ModuleManagerLoaded;
}
