/*
 * stylable: An interface which can be inherited by actor and objects
 *           to get styled by a theme
 * 
 * Copyright 2012-2014 Stephan Haller <nomad@froevel.de>
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

#include "stylable.h"

#include <glib/gi18n-lib.h>

/* Define this interface in GObject system */
G_DEFINE_INTERFACE(XfdashboardStylable,
					xfdashboard_stylable,
					G_TYPE_OBJECT)

/* IMPLEMENTATION: Private variables and methods */
#define XFDASHBOARD_STYLABLE_WARN_NOT_IMPLEMENTED(self, vfunc) \
	g_warning(_("Object of type %s does not implement required virtual function XfdashboardStylable::%s"), \
				G_OBJECT_TYPE_NAME(self), \
				vfunc);

/* Check if haystack contains needle.
 * The haystack is a string representing a list which entries is seperated
 * by a seperator character. This function looks up the haystack if it
 * contains an entry matching the needle and returns TRUE in this case.
 * Otherwise FALSE is returned. A needle length of -1 signals that needle
 * is a NULL-terminated string and length should be determine automatically.
 */
static gboolean _xfdashboard_stylable_list_contains(const gchar *inNeedle,
													gint inNeedleLength,
													const gchar *inHaystack,
													gchar inSeperator)
{
	const gchar		*start;

	g_return_val_if_fail(inNeedle && *inNeedle!=0, FALSE);
	g_return_val_if_fail(inNeedleLength>0 || inNeedleLength==-1, FALSE);
	g_return_val_if_fail(inHaystack && *inHaystack!=0, FALSE);
	g_return_val_if_fail(inSeperator, FALSE);

	/* If given length of needle is negative it is a NULL-terminated string */
	if(inNeedleLength<0) inNeedleLength=strlen(inNeedle);

	/* Lookup needle in haystack */
	for(start=inHaystack; start; start=strchr(start, inSeperator))
	{
		gint		length;
		gchar		*nextEntry;

		/* Move to character after separator */
		if(start[0]==inSeperator) start++;

		/* Find end of this haystack entry */
		nextEntry=strchr(start, inSeperator);
		if(!nextEntry) length=strlen(start);
			else length=nextEntry-start;

		/* If enrty in haystack is not of same length as needle,
		 * then it is not a match
		 */
		if(length!=inNeedleLength) continue;

		if(!strncmp(inNeedle, start, inNeedleLength)) return(TRUE);
	}

	/* Needle was not found */
	return(FALSE);
}

/* IMPLEMENTATION: GObject */

/* Interface initialization
 * Set up default functions
 */
void xfdashboard_stylable_default_init(XfdashboardStylableInterface *iface)
{
	GParamSpec			*property;

	/* All virtual function must be overridden */
	iface->get_name=NULL;
	iface->get_parent=NULL;
	iface->get_classes=NULL;
	iface->set_classes=NULL;
	iface->get_pseudo_classes=NULL;
	iface->set_pseudo_classes=NULL;
	iface->invalidate=NULL;

	/* Define properties */
	property=g_param_spec_string("style-classes",
									_("Style classes"),
									_("String representing list of classes separated by '.'"),
									NULL,
									G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	g_object_interface_install_property(iface, property);

	property=g_param_spec_string("style-pseudo-classes",
									_("Style pseudo-classes"),
									_("String representing list of pseudo-classes, e.g. current state, separated by ':'"),
									NULL,
									G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	g_object_interface_install_property(iface, property);
}

/* Implementation: Public API */

/* Call virtual function "get_name" */
const gchar* xfdashboard_stylable_get_name(XfdashboardStylable *self)
{
	XfdashboardStylableInterface		*iface;

	g_return_val_if_fail(XFDASHBOARD_IS_STYLABLE(self), NULL);

	iface=XFDASHBOARD_STYLABLE_GET_IFACE(self);

	/* Call virtual function */
	if(iface->get_name)
	{
		return(iface->get_name(self));
	}

	/* If we get here the virtual function was not overridden */
	XFDASHBOARD_STYLABLE_WARN_NOT_IMPLEMENTED(self, "get_name");
	return(NULL);
}

/* Call virtual function "get_parent" */
XfdashboardStylable* xfdashboard_stylable_get_parent(XfdashboardStylable *self)
{
	XfdashboardStylableInterface		*iface;

	g_return_val_if_fail(XFDASHBOARD_IS_STYLABLE(self), NULL);

	iface=XFDASHBOARD_STYLABLE_GET_IFACE(self);

	/* Call virtual function */
	if(iface->get_parent)
	{
		return(XFDASHBOARD_STYLABLE(iface->get_parent(self)));
	}

	/* If we get here the virtual function was not overridden */
	XFDASHBOARD_STYLABLE_WARN_NOT_IMPLEMENTED(self, "get_parent");
	return(NULL);
}

/* Call virtual function "get_classes" */
const gchar* xfdashboard_stylable_get_classes(XfdashboardStylable *self)
{
	XfdashboardStylableInterface		*iface;

	g_return_val_if_fail(XFDASHBOARD_IS_STYLABLE(self), NULL);

	iface=XFDASHBOARD_STYLABLE_GET_IFACE(self);

	/* Call virtual function */
	if(iface->get_classes)
	{
		return(iface->get_classes(self));
	}

	/* If we get here the virtual function was not overridden */
	XFDASHBOARD_STYLABLE_WARN_NOT_IMPLEMENTED(self, "get_classes");
	return(NULL);
}

/* Call virtual function "set_classes" */
void xfdashboard_stylable_set_classes(XfdashboardStylable *self, const gchar *inClasses)
{
	XfdashboardStylableInterface		*iface;

	g_return_if_fail(XFDASHBOARD_IS_STYLABLE(self));

	iface=XFDASHBOARD_STYLABLE_GET_IFACE(self);

	/* Call virtual function */
	if(iface->set_classes)
	{
		iface->set_classes(self, inClasses);
		return;
	}

	/* If we get here the virtual function was not overridden */
	XFDASHBOARD_STYLABLE_WARN_NOT_IMPLEMENTED(self, "set_classes");
}

/* Adds a class to existing classes of an object */
void xfdashboard_stylable_add_class(XfdashboardStylable *self, const gchar *inClass)
{
	const gchar		*classes;

	g_return_if_fail(XFDASHBOARD_IS_STYLABLE(self));
	g_return_if_fail(inClass && inClass[0]);

	/* If class is already in list of classes do nothing otherwise set new value */
	classes=xfdashboard_stylable_get_classes(self);
	if(!classes ||
		!_xfdashboard_stylable_list_contains(inClass, -1, classes, '.'))
	{
		gchar					*newClasses;

		/* Create new temporary string by concatenating current classes
		 * and new class with dot separator. Set this new string representing
		 * list of classes.
		 */
		if(classes) newClasses=g_strconcat(classes, ".", inClass, NULL);
			else newClasses=g_strdup(inClass);

		xfdashboard_stylable_set_classes(self, newClasses);

		g_free(newClasses);
	}
}

/* Removed a class to existing classes of an object */
void xfdashboard_stylable_remove_class(XfdashboardStylable *self, const gchar *inClass)
{
	const gchar		*classes;

	g_return_if_fail(XFDASHBOARD_IS_STYLABLE(self));
	g_return_if_fail(inClass && inClass[0]);

	/* If class is not in list of classes do nothing otherwise set new value */
	classes=xfdashboard_stylable_get_classes(self);
	if(classes &&
		_xfdashboard_stylable_list_contains(inClass, -1, classes, '.'))
	{
		gchar					**oldClasses, **entry;
		gchar					*newClasses, *newClassesTemp;

		/* Create new temporary string with all current classes separated by dot
		 * not matching class to remove. Set this new string representing list
		 * of classes.
		 */
		entry=oldClasses=g_strsplit(classes, ".", -1);
		newClasses=NULL;
		while(*entry)
		{
			if(!strcmp(*entry, inClass))
			{
				entry++;
				continue;
			}

			if(newClasses)
			{
				newClassesTemp=g_strconcat(newClasses, ".", *entry, NULL);
				g_free(newClasses);
				newClasses=newClassesTemp;
			}
				else newClasses=g_strdup(*entry);

			entry++;
		}

		xfdashboard_stylable_set_classes(self, newClasses);

		g_strfreev(oldClasses);
		g_free(newClasses);
	}
}

/* Call virtual function "get_pseudo_classes" */
const gchar* xfdashboard_stylable_get_pseudo_classes(XfdashboardStylable *self)
{
	XfdashboardStylableInterface		*iface;

	g_return_val_if_fail(XFDASHBOARD_IS_STYLABLE(self), NULL);

	iface=XFDASHBOARD_STYLABLE_GET_IFACE(self);

	/* Call virtual function */
	if(iface->get_pseudo_classes)
	{
		return(iface->get_pseudo_classes(self));
	}

	/* If we get here the virtual function was not overridden */
	XFDASHBOARD_STYLABLE_WARN_NOT_IMPLEMENTED(self, "get_pseudo_classes");
	return(NULL);
}

/* Call virtual function "set_pseudo_classes" */
void xfdashboard_stylable_set_pseudo_classes(XfdashboardStylable *self, const gchar *inClasses)
{
	XfdashboardStylableInterface		*iface;

	g_return_if_fail(XFDASHBOARD_IS_STYLABLE(self));

	iface=XFDASHBOARD_STYLABLE_GET_IFACE(self);

	/* Call virtual function */
	if(iface->set_pseudo_classes)
	{
		iface->set_pseudo_classes(self, inClasses);
		return;
	}

	/* If we get here the virtual function was not overridden */
	XFDASHBOARD_STYLABLE_WARN_NOT_IMPLEMENTED(self, "set_pseudo_classes");
}

/* Adds a pseudo-class to existing pseudo-classes of an object */
void xfdashboard_stylable_add_pseudo_class(XfdashboardStylable *self, const gchar *inClass)
{
	const gchar		*classes;

	g_return_if_fail(XFDASHBOARD_IS_STYLABLE(self));
	g_return_if_fail(inClass && inClass[0]);

	/* If pesudo-class is already in list of pseudo-classes do nothing
	 * otherwise set new value.
	 */
	classes=xfdashboard_stylable_get_pseudo_classes(self);
	if(!classes ||
		!_xfdashboard_stylable_list_contains(inClass, -1, classes, ':'))
	{
		gchar					*newClasses;

		/* Create new temporary string by concatenating current pseudo-classes
		 * and new psuedo-class with colon separator. Set this new string
		 * representing list of pseudo-classes.
		 */
		if(classes) newClasses=g_strconcat(classes, ":", inClass, NULL);
			else newClasses=g_strdup(inClass);

		xfdashboard_stylable_set_pseudo_classes(self, newClasses);

		g_free(newClasses);
	}
}

/* Removed a pseudo-class to existing classes of an object */
void xfdashboard_stylable_remove_pseudo_class(XfdashboardStylable *self, const gchar *inClass)
{
	const gchar		*classes;

	g_return_if_fail(XFDASHBOARD_IS_STYLABLE(self));
	g_return_if_fail(inClass && inClass[0]);

	/* If pseudo-class is not in list of pseudo-classes do nothing
	 * otherwise set new value.
	 */
	classes=xfdashboard_stylable_get_pseudo_classes(self);
	if(classes &&
		_xfdashboard_stylable_list_contains(inClass, -1, classes, ':'))
	{
		gchar					**oldClasses, **entry;
		gchar					*newClasses, *newClassesTemp;

		/* Create new temporary string with all current pseudo-classes
		 * separated by colon not matching pseudo-class to remove.
		 * Set this new string representing list of pseudo-classes.
		 */
		entry=oldClasses=g_strsplit(classes, ":", -1);
		newClasses=NULL;
		while(*entry)
		{
			if(!strcmp(*entry, inClass))
			{
				entry++;
				continue;
			}

			if(newClasses)
			{
				newClassesTemp=g_strconcat(newClasses, ":", *entry, NULL);
				g_free(newClasses);
				newClasses=newClassesTemp;
			}
				else newClasses=g_strdup(*entry);

			entry++;
		}

		xfdashboard_stylable_set_pseudo_classes(self, newClasses);

		g_strfreev(oldClasses);
		g_free(newClasses);
	}
}

/* Call virtual function "invalidate" */
void xfdashboard_stylable_invalidate(XfdashboardStylable *self)
{
	XfdashboardStylableInterface		*iface;

	g_return_if_fail(XFDASHBOARD_IS_STYLABLE(self));

	iface=XFDASHBOARD_STYLABLE_GET_IFACE(self);

	/* Call virtual function */
	if(iface->invalidate)
	{
		iface->invalidate(self);
		return;
	}

	/* If we get here the virtual function was not overridden */
	XFDASHBOARD_STYLABLE_WARN_NOT_IMPLEMENTED(self, "invalidate");
}