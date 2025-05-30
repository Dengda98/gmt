#
# Copyright (c) 1991-2025 by the GMT Team (https://www.generic-mapping-tools.org/team.html)
# See LICENSE.TXT file for copying and redistribution conditions.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; version 3 or any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# Contact info: www.generic-mapping-tools.org
#-------------------------------------------------------------------------------

if (SPHINX_FOUND)
	# Copy PS sources for gmt release
	if (GIT_FOUND AND HAVE_GIT_VERSION)
		add_custom_target (_scripts_images_release
			COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_CURRENT_SOURCE_DIR}/images/
			${GMT_RELEASE_PREFIX}/doc/scripts/images
			DEPENDS git_export_release)
		add_depend_to_target (gmt_release _scripts_images_release)
	endif (GIT_FOUND AND HAVE_GIT_VERSION)

	if (GMT_DOCS_DEPEND_ON_GMT)
		set (_gmt_command ${GMT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/src/gmt)
	else (GMT_DOCS_DEPEND_ON_GMT)
		set (_gmt_command gmt)
	endif (GMT_DOCS_DEPEND_ON_GMT)

	# Convert PS to PNG
	file (GLOB _scripts_ps2png RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/images/*.ps")
	set (_scripts_png)
	foreach (_ps ${_scripts_ps2png})
		get_filename_component (_fig ${_ps} NAME)
		string (REPLACE ".ps" ".png" _png_fig ${_fig})
		list (APPEND _scripts_png ${RST_BINARY_DIR}/_images/${_png_fig})
		add_custom_command (OUTPUT ${RST_BINARY_DIR}/_images/${_png_fig}
			COMMAND ${CMAKE_COMMAND} -E env
			GMT_SHAREDIR=${GMT_SOURCE_DIR}/share
			${_gmt_command} psconvert
			-A -P -E150 -Tg -Qg4 -Qt4
			-D${RST_BINARY_DIR}/_images
			${CMAKE_CURRENT_SOURCE_DIR}/${_ps}
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			DEPENDS gmt_for_img_convert _docs_rst_mkdir_images ${CMAKE_CURRENT_SOURCE_DIR}/${_ps})
	endforeach (_ps ${_scripts_ps2png})

	# Convert PS to PDF
	set (_scripts_ps2pdf images/GMT_RGBchart.ps images/GMT_App_F_stand+_iso+.ps images/GMT_App_F_symbol_dingbats.ps)
	set (_scripts_pdf)
	foreach (_ps ${_scripts_ps2pdf})
		get_filename_component (_fig ${_ps} NAME)
		string (REPLACE ".ps" ".pdf" _pdf_fig ${_fig})
		list (APPEND _scripts_pdf ${RST_BINARY_DIR}/_images/${_pdf_fig})
		add_custom_command (OUTPUT ${RST_BINARY_DIR}/_images/${_pdf_fig}
			COMMAND ${CMAKE_COMMAND} -E env
			GMT_SHAREDIR=${GMT_SOURCE_DIR}/share
			${_gmt_command} psconvert -A -P -Tf
			-D${RST_BINARY_DIR}/_images
			${CMAKE_CURRENT_SOURCE_DIR}/${_ps}
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			DEPENDS gmt_for_img_convert _docs_rst_mkdir_images ${CMAKE_CURRENT_SOURCE_DIR}/${_ps})
	endforeach (_ps ${_scripts_ps})

	# Convert script to verbatim txt
	file (GLOB _scripts_sh2txt RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/*.sh")
	set (_scripts_txt)
	foreach (_script ${_scripts_sh2txt})
		string (REPLACE ".sh" ".txt" _txt ${_script})
		list (APPEND _scripts_txt ${RST_BINARY_DIR}/_verbatim/${_txt})
		add_custom_command (OUTPUT ${RST_BINARY_DIR}/_verbatim/${_txt}
			COMMAND ${GMT_BINARY_DIR}/src/script2verbatim --strip-comments --ps2pdf
			${CMAKE_CURRENT_SOURCE_DIR}/${_script}
			${RST_BINARY_DIR}/_verbatim/${_txt}
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			DEPENDS script2verbatim _docs_rst_mkdir_verbatim ${CMAKE_CURRENT_SOURCE_DIR}/${_script})
	endforeach (_script)

	# Add build target
	add_custom_target (_docs_scripts_verbatim DEPENDS ${_scripts_txt})
	add_custom_target (_docs_html_scripts_fig DEPENDS ${_scripts_png} ${_scripts_pdf})
	add_depend_to_target (docs_depends _docs_html_scripts_fig _docs_scripts_verbatim)
endif (SPHINX_FOUND)

# run tests
file (GLOB _scripts_tests RELATIVE ${CMAKE_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/*.sh")
# GMT_-U.sh always fails
foreach (skip_test GMT_-U.sh GMT_encoding.sh gen_data_App_O.sh gen_data_dummy.sh)
	list (REMOVE_ITEM _scripts_tests doc/scripts/${skip_test})
endforeach()

# Remove GMT_latex.sh if latex or dvips not found
if (NOT LATEX_FOUND)
	list (REMOVE_ITEM _scripts_tests doc/scripts/GMT_latex.sh doc/scripts/GMT_slope2intensity.sh doc/scripts/GMT_seamount_density.sh)
endif (NOT LATEX_FOUND)

if (DO_TESTS AND BASH)
	foreach (_job ${_scripts_tests})
		add_test (NAME ${_job}
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMAND ${BASH} ${CMAKE_BINARY_DIR}/test/gmtest ${_job})
	endforeach (_job)
endif (DO_TESTS AND BASH)
