/*--------------------------------------------------------------------
 *
 *	Copyright (c) 1991-2025 by the GMT Team (https://www.generic-mapping-tools.org/team.html)
 *	See LICENSE.TXT file for copying and redistribution conditions.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as published by
 *	the Free Software Foundation; version 3 or any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	Contact info: www.generic-mapping-tools.org
 *--------------------------------------------------------------------*/
/*
 * gmt_dwc.h contains definitions for using the DCW in GMT.
 * DCW - The Digital Chart of the World
 *
 * Author:	Paul Wessel
 * Date:	10-MAR-2013
 * Version:	6 API
 */

/*!
 * \file gmt_dcw.h
 * \brief Definitions for using the DCW
 */

#ifndef GMT_DCW_H
#define GMT_DCW_H

#define DCW_OPT "<code1,code2,...>[+f<file>][+l|L|n][+c|C][+g<fill>][+p<pen>][+z]"

enum GMT_DCW_modes {
	GMT_DCW_REGION	 = 1,
	GMT_DCW_PLOT     = 2,
	GMT_DCW_CLIP_IN  = 4,
	GMT_DCW_CLIP_OUT = 8,
	GMT_DCW_DUMP	 = 16,
	GMT_DCW_EXTRACT	 = 32,
	GMT_DCW_LIST	 = 64,
	GMT_DCW_ZHEADER	 = 128
};

struct GMT_DCW_ITEM {	/* One set of codes with their color/fill */
	char *codes;		/* Comma separated list of codes with modifiers */
	unsigned int mode;	/* 4 = outline, 8 = fill */
	struct GMT_PEN pen;	/* Pen for outline [no outline] */
	struct GMT_FILL fill;	/* Fill for polygons */
};

struct GMT_DCW_SELECT {	/* -F<DWC-options> */
	bool region;		/* Determine region from polygons instead of -R */
	double inc[4];		/* Increments for rounded region */
	unsigned int adjust;		/* Round/adjust the region from polygons using the incs */
	unsigned int mode;	/* 1 get countries, 2 get countries and states */
	unsigned int n_items;	/* Number of items (times) -F was given */
	char *other_dcw_file;		/* Name of alternative dcw file (NULL if not used) */
	struct GMT_DCW_ITEM **item;	/* Pointer to array of n_items items */
	struct GMT_OPTION *options;	/* Pointer to the GMT options */
};

EXTERN_MSC int gmt_DCW_version(struct GMTAPI_CTRL *API, char *version);
EXTERN_MSC unsigned int gmt_DCW_list(struct GMT_CTRL *GMT, struct GMT_DCW_SELECT *F);
EXTERN_MSC unsigned int gmt_DCW_parse(struct GMT_CTRL *GMT, char option, char *args, struct GMT_DCW_SELECT *F);
EXTERN_MSC void gmt_DCW_option(struct GMTAPI_CTRL *API, char option, unsigned int plot);
EXTERN_MSC struct GMT_DATASET *gmt_DCW_operation(struct GMT_CTRL *GMT, struct GMT_DCW_SELECT *F, double wesn[], unsigned int mode);
EXTERN_MSC void gmt_DCW_free(struct GMT_CTRL *GMT, struct GMT_DCW_SELECT *F);

#endif /* GMT_DCW_H */
