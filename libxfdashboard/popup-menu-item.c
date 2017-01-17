/*
 * popup-menu-item: An interface implemented by actors used as pop-up menu item
 * 
 * Copyright 2012-2017 Stephan Haller <nomad@froevel.de>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <libxfdashboard/popup-menu-item.h>

#include <glib/gi18n-lib.h>

#include <libxfdashboard/compat.h>


/* Define this interface in GObject system */
G_DEFINE_INTERFACE(XfdashboardPopupMenuItem,
					xfdashboard_popup_menu_item,
					G_TYPE_OBJECT)

/* Signals */
enum
{
	/* Signals */
	SIGNAL_ACTIVATED,

	SIGNAL_LAST
};

static guint XfdashboardPopupMenuItemSignals[SIGNAL_LAST]={ 0, };

/* IMPLEMENTATION: Private variables and methods */
#define XFDASHBOARD_POPUP_MENU_ITEM_WARN_NOT_IMPLEMENTED(self, vfunc) \
	g_warning(_("Object of type %s does not implement required virtual function XfdashboardPopupMenuItem::%s"), \
				G_OBJECT_TYPE_NAME(self), \
				vfunc);

/* Default implementation of virtual function "get_enabled" */
static gboolean _xfdashboard_popup_menu_item_real_get_enabled(XfdashboardPopupMenuItem *self)
{
	gboolean			enabled;

	g_return_val_if_fail(XFDASHBOARD_IS_POPUP_MENU_ITEM(self), FALSE);

	/* By default (if not overidden) retrieve state from property "enabled" */
	g_object_get(self, "enabled", &enabled, NULL);

	return(enabled);
}

/* Default implementation of virtual function "set_enabled" */
static void _xfdashboard_popup_menu_item_real_set_enabled(XfdashboardPopupMenuItem *self, gboolean inEnabled)
{
	g_return_if_fail(XFDASHBOARD_IS_POPUP_MENU_ITEM(self));

	/* By default (if not overidden) set property "enabled" to requested state */
	g_object_set(self, "enabled", inEnabled, NULL);
}

/* IMPLEMENTATION: GObject */

/* Interface initialization
 * Set up default functions
 */
void xfdashboard_popup_menu_item_default_init(XfdashboardPopupMenuItemInterface *iface)
{
	static gboolean		initialized=FALSE;
	GParamSpec			*property;

	/* The following virtual functions should be overriden if default
	 * implementation does not fit.
	 */
	iface->get_enabled=_xfdashboard_popup_menu_item_real_get_enabled;
	iface->set_enabled=_xfdashboard_popup_menu_item_real_set_enabled;

	/* Define properties, signals and actions */
	if(!initialized)
	{
		/* Define properties */
		/**
		 * XfdashboardPopupMenuItem:enabled:
		 *
		 * A flag indicating if this pop-up menu item is enabled or disabled.
		 * It is set to %TRUE if enabled and it is set %FALSE if disabled.
		 */
		property=g_param_spec_boolean("enabled",
										_("Enabled"),
										_("Whether this pop-up menu item is enabled"),
										TRUE,
										G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
		g_object_interface_install_property(iface, property);

		/* Define signals */
		/**
		 * XfdashboardPopupMenuItem::activated:
		 * @self: The pop-up menu item which was activated
		 *
		 * The ::activated signal is emitted for the item the user selected in
		 * the pop-up menu.
		 */
		XfdashboardPopupMenuItemSignals[SIGNAL_ACTIVATED]=
			g_signal_new("activated",
							G_TYPE_FROM_INTERFACE(iface),
							G_SIGNAL_RUN_LAST,
							0,
							NULL,
							NULL,
							g_cclosure_marshal_VOID__VOID,
							G_TYPE_NONE,
							0);

		/* Set flag that base initialization was done for this interface */
		initialized=TRUE;
	}
}

/* IMPLEMENTATION: Public API */

/**
 * xfdashboard_popup_menu_item_get_enabled:
 * @self: A #XfdashboardPopupMenuItem
 *
 * Retrieves the state of pop-up menu item at @self if it is enabled or disabled.
 * If %TRUE is returned this item is enabled and can be selected, focused and
 * activated. If this item is disabled, %FALSE is returned and it is not possible
 * to interact with this item.
 *
 * Return value: %TRUE if pop-up menu item at @self is enabled and %FALSE if disabled
 */
gboolean xfdashboard_popup_menu_item_get_enabled(XfdashboardPopupMenuItem *self)
{
	XfdashboardPopupMenuItemInterface		*iface;

	g_return_val_if_fail(XFDASHBOARD_IS_POPUP_MENU_ITEM(self), FALSE);

	iface=XFDASHBOARD_POPUP_MENU_ITEM_GET_IFACE(self);

	/* Call virtual function */
	if(iface->get_enabled)
	{
		return(iface->get_enabled(self));
	}

	/* If we get here the virtual function was not overridden */
	XFDASHBOARD_POPUP_MENU_ITEM_WARN_NOT_IMPLEMENTED(self, "get_enabled");
	return(FALSE);
}

/**
 * xfdashboard_popup_menu_item_set_enabled:
 * @self: A #XfdashboardPopupMenuItem
 * @inEnabled: A boolean flag if this pop-up menu item should be enabled or disabled
 * 
 * Sets the state of pop-up menu item at @self to the state at @inEnabled.
 * If @inEnabled is %TRUE this item will be enabled and will be selectable, focusable
 * and activatable. If @inEnabled is %FALSE this item will be disabled and it will
 * not possible to interact with this item.
 */
void xfdashboard_popup_menu_item_set_enabled(XfdashboardPopupMenuItem *self, gboolean inEnabled)
{
	XfdashboardPopupMenuItemInterface		*iface;

	g_return_if_fail(XFDASHBOARD_IS_POPUP_MENU_ITEM(self));

	iface=XFDASHBOARD_POPUP_MENU_ITEM_GET_IFACE(self);

	/* Call virtual function */
	if(iface->set_enabled)
	{
		iface->set_enabled(self, inEnabled);
		return;
	}

	/* If we get here the virtual function was not overridden */
	XFDASHBOARD_POPUP_MENU_ITEM_WARN_NOT_IMPLEMENTED(self, "set_enabled");
}

/**
 * xfdashboard_popup_menu_item_activate:
 * @self: A #XfdashboardPopupMenuItem
 *
 * Activates the menu item at @self by emitting the signal "activated".
 */
void xfdashboard_popup_menu_item_activate(XfdashboardPopupMenuItem *self)
{
	g_return_if_fail(XFDASHBOARD_IS_POPUP_MENU_ITEM(self));

	/* Check if popup menu item is enabled and return if disabled */
	if(!xfdashboard_popup_menu_item_get_enabled(self)) return;

	/* Emit signal for activation */
	g_signal_emit(self, XfdashboardPopupMenuItemSignals[SIGNAL_ACTIVATED], 0);
}