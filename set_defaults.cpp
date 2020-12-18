
/*********************************************************************************************

    This is public domain software that was developed by or for the U.S. Naval Oceanographic
    Office and/or the U.S. Army Corps of Engineers.

    This is a work of the U.S. Government. In accordance with 17 USC 105, copyright protection
    is not available for any work of the U.S. Government.

    Neither the United States Government, nor any employees of the United States Government,
    nor the author, makes any warranty, express or implied, without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE, or assumes any liability or
    responsibility for the accuracy, completeness, or usefulness of any information,
    apparatus, product, or process disclosed, or represents that its use would not infringe
    privately-owned rights. Reference herein to any specific commercial products, process,
    or service by trade name, trademark, manufacturer, or otherwise, does not necessarily
    constitute or imply its endorsement, recommendation, or favoring by the United States
    Government. The views and opinions of authors expressed herein do not necessarily state
    or reflect those of the United States Government, and shall not be used for advertising
    or product endorsement purposes.
*********************************************************************************************/


/****************************************  IMPORTANT NOTE  **********************************

    Comments in this file that start with / * ! or / / ! are being used by Doxygen to
    document the software.  Dashes in these comment blocks are used to create bullet lists.
    The lack of blank lines after a block of dash preceeded comments means that the next
    block of dash preceeded comments is a new, indented bullet list.  I've tried to keep the
    Doxygen formatting to a minimum but there are some other items (like <br> and <pre>)
    that need to be left alone.  If you see a comment that starts with / * ! or / / ! and
    there is something that looks a bit weird it is probably due to some arcane Doxygen
    syntax.  Be very careful modifying blocks of Doxygen comments.

*****************************************  IMPORTANT NOTE  **********************************/



#include "mosaicView.hpp"


void set_defaults (MISC *misc, OPTIONS *options, uint8_t restore)
{
  //  Set Defaults so that if keys don't for envin the parms are defined.

  if (!restore)
    {
      misc->poly_count = 0;
      misc->feature = NULL;
      misc->nearest_feature = -1;
      misc->feature_search_string = "";

      memset (&misc->bfd_header, 0, sizeof (BFDATA_HEADER));

      misc->bfd_open = NVFalse;
    }


  options->coast = NVFalse;
  options->startup_message = NVTrue;
  options->position_form = 0;
  options->zoom_percent = 10;
  options->width = 1024;
  options->height = 768;
  options->window_x = 0;
  options->window_y = 0;
  options->marker_color = QColor (255, 0, 0, 128);
  options->coast_color = QColor (255, 255, 0);
  options->display_feature = NVFalse;
  options->display_children = NVFalse;
  options->display_feature_info = NVFalse;
  options->display_feature_poly = NVFalse;
  options->rect_color = QColor (255, 255, 255);
  options->feature_color = QColor (255, 255, 255);
  options->feature_info_color = QColor (255, 255, 255);
  options->feature_poly_color = QColor (255, 255, 0, 255);
  options->highlight_color = QColor (255, 0, 0);
  options->coast_color = QColor (255, 255, 0);
  options->font = QApplication::font ();
  options->feature_diameter = 20.0;
  options->last_rock_feature_desc = 4;
  options->last_offshore_feature_desc = 17;
  options->last_light_feature_desc = 9;
  options->last_lidar_feature_desc = 0;
  options->last_feature_description = "";
  options->last_feature_remarks = "";
  options->screenshot_delay = 2;
}
