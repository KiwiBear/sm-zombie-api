/*
 * ============================================================================
 *
 *  Zombie API
 *
 *  File:           dependencymanager.inc
 *  Description:    API for handling dependencies.
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

#if defined _zm_dependencymanager_included
 #endinput
#endif
#define _zm_dependencymanager_included

/*____________________________________________________________________________*/

/**
 * Predefined module manager library name to be used with OnLibraryAdded,
 * OnLibraryRemoved, and LibraryExists.
 */
#define LIBRARY_ZM_DEPENDENCY_MANAGER "zm_dependency_manager"

/*____________________________________________________________________________*/

/**
 * @section Events
 */

/**
 * All dependencies are ready. The plugin may initialize now.
 *
 * @noreturn
 */
#define ZM_ON_DEPENDENCIES_READY "OnDependenciesReady"
functag public ZM_OnDependenciesReady();

/**
 * One or more dependencies is being unloaded. The plugin should clean up
 * resources and disable itself. It may also wait for OnDependenciesReady so it
 * can reinitialize itself again.
 *
 * This event is only triggered after OnDependenciesReady, never before.
 *
 * @noreturn
 */
#define ZM_ON_DEPENDENCIES_UNAVAILABLE "OnDependenciesUnavailable"
functag public ZM_OnDependenciesUnavailable();

/**
 * @endsection
 */

/*____________________________________________________________________________*/

/**
 * Sets callbacks to watch for dependency changes.
 *
 * @param ready         Function to call when all dependencies are ready.
 * @param unavailable   Function to call when one or more dependencies are being
 *                      unloaded.
 *
 * @noreturn
 * @error               Invalid callback.
 */
native ZM_SetDependencyCallbacks(
        ZM_OnDependenciesReady:ready,
        ZM_OnDependenciesUnavailable:unavailable);

/*____________________________________________________________________________*/

/**
 * Declares a dependency on the specified library.
 *
 * The dependency manager will add the library to the calling plugin's
 * dependency list.
 *
 * Warning: The dependency manager doesn't account for circular dependencies, so
 *          the behavior in those cases is undefined. Most likely it will result
 *          in a deadlock (nothing happens).
 *
 * @param libraryName       Name of library to depend on. This should be the
 *                          name that libraries send to RegPluginLibrary.
 *
 * @noreturn
 */
native ZM_DependsOn(const String:libraryName[]);

/*____________________________________________________________________________*/

/**
 * Removes the calling plugin and its dependency declarations from the
 * dependency manager.
 *
 * This will clean up resources you've allocated in the dependency manager.
 *
 * Warning: You must call this function in OnPluginEnd or earlier to release
 *          resources you've allocated before your plugin is unloaded. Otherwise
 *          the dependency manager will leak memory.
 *
 * @noreturn
 */
native ZM_RemoveDependencies();

/*____________________________________________________________________________*/

/**
 * Adds a named library. It must match the name used in RegPluginLibrary so the
 * dependency manager will detect changes.
 *
 * @param libraryName       Unique name of library.
 * @param manualReadyEvent  (Optional) Whether the library plugin should trigger
 *                          the ready-event manually. Default is false;
 *                          automatically trigger ready-event based on the
 *                          OnLibraryAdded forward and LibraryExist function.
 *
 * @noreturn
 * @error                   Library name is already added. It's a conflict or a
 *                          duplicate.
 */
native ZM_AddLibrary(const String:libraryName[], bool:manualReadyEvent = false);

/*____________________________________________________________________________*/

/**
 * Removes the specified library. If any clients has declared dependency on this
 * library, it will first trigger the unavailable-event and then release
 * allocated resources.
 *
 * Warning: Libraries must call this when the plugin is unloading. Otherwise
 *          there will be a memory leak in the dependency manager.
 *
 * @param libraryName       Library to remove.
 *
 * @noreturn
 * @error                   Caller is not the library owner. The library was
 *                          added by another plugin.
 */
native ZM_RemoveLibrary(const String:libraryName[]);

/*____________________________________________________________________________*/

/**
 * Removes all libraries registered by the calling plugin and triggers the
 * unavailable event if any clients depends on them.
 *
 * If the caller has no libraries, it does nothing.
 *
 * @noreturn
 */
native ZM_RemoveLibraries();

/*____________________________________________________________________________*/

/**
 * Returns whether the specified library has been added.
 *
 * @param libraryName       Library name to search for.
 *
 * @return                  True if added, false otherwise.
 */
native bool:ZM_HasLibrary(const String:libraryName[]);

/*____________________________________________________________________________*/

/**
 * Manually trigger the ready-event for the specified library, if not already
 * ready.
 *
 * @param libraryName       Name of library that's ready.
 *
 * @return                  True if ready event was triggered, false otherwise.
 * @error                   Caller is not the library owner. The library was
 *                          added by another plugin.
 */
native bool:ZM_LibraryReady(const String:libraryName[]);

/*____________________________________________________________________________*/

/**
 * Manually trigger the unavailable-event for the specified library, if not
 * already unavailable. Libraries may call this if they need to unload early.
 *
 * @param libraryName       Name of library that's unavailable.
 *
 * @return                  True if unavailable-event was triggered, false
 *                          otherwise.
 * @error                   Caller is not the library owner. The library was
 *                          added by another plugin.
 */
native bool:ZM_LibraryUnavailable(const String:libraryName[]);

/*____________________________________________________________________________*/

/**
 * Marks all natives in the module manager as optional.
 */
stock ZM_SetDependencyManagerOptional()
{
    MarkNativeAsOptional("ZM_SetDependencyCallbacks");
    MarkNativeAsOptional("ZM_DependsOn");
    MarkNativeAsOptional("ZM_RemoveDependencies");
    MarkNativeAsOptional("ZM_AddLibrary");
    MarkNativeAsOptional("ZM_RemoveLibrary");
    MarkNativeAsOptional("ZM_HasLibrary");
    MarkNativeAsOptional("ZM_LibraryReady");
    MarkNativeAsOptional("ZM_LibraryUnavailable");
}
