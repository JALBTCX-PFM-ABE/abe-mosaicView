
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


void overlayFlag (nvMap *map, OPTIONS *options, MISC *misc)
{
  int32_t             i, x, y, z;
  double              wlon, elon, slat, nlat, dep, nx[2], ny[2];
  QColor              tmp;



  slat = misc->displayed_area.min_y;
  nlat = misc->displayed_area.max_y;
  wlon = misc->displayed_area.min_x;
  elon = misc->displayed_area.max_x;


  //  Display features.

  if (options->display_feature)
    {
      tmp = options->feature_color;
      if (misc->abe_share->layer_type == NUM_LAYERS) tmp = Qt::black;

      for (i = 0 ; i < (int32_t) misc->bfd_header.number_of_records ; i++)
        {
          if (misc->feature[i].confidence_level || misc->abe_share->layer_type == AVERAGE_DEPTH || 
              misc->abe_share->layer_type == MIN_DEPTH || misc->abe_share->layer_type == MAX_DEPTH)
            {
              if (misc->feature[i].longitude >= wlon && misc->feature[i].longitude <= elon &&
                  misc->feature[i].latitude >= slat && misc->feature[i].latitude <= nlat)
                {
                  //  Check to see if this is a child record.

                  if (options->display_children || !misc->feature[i].parent_record)
                    {
                      map->map_to_screen (1, &misc->feature[i].longitude, &misc->feature[i].latitude, &dep, &x, &y, &z);


                      //  Go through here if we want to display feature info, poly, or we want to
                      //  highlight using a feature text search.

                      if (options->display_feature_info || options->display_feature_poly ||
                          !misc->feature_search_string.isEmpty ())
                        {
                          QString string0 (misc->feature[i].description);

                          QString string1 (misc->feature[i].remarks);

                          QString new_string;
                          if (string1.isEmpty ())
                            {
                              if (!string0.isEmpty ()) new_string = string0;
                            }
                          else
                            {
                              if (string0.isEmpty ())
                                {
                                  new_string = string1;
                                }
                              else
                                {
                                  new_string = string0 + " :: " + string1;
                                }
                            }


                          tmp = options->feature_info_color;
                          if (options->display_feature_info && !new_string.isEmpty ()) 
                            map->drawText (new_string, x + 6, y + 3, tmp, NVFalse);


                          QString low_string = new_string.toLower ();
                          if (!misc->feature_search_string.isEmpty () && 
                              low_string.contains (misc->feature_search_string.toLower ())) 
                            {
                              tmp = options->highlight_color;
                            }


                          //  Feature polygons.

                          if (options->display_feature_poly && misc->feature[i].poly_count)
                            {
                              BFDATA_POLYGON bfd_polygon;

                              binaryFeatureData_read_polygon (misc->bfd_handle, misc->feature[i].record_number, &bfd_polygon);


                              //  Draw a line from the center of the associated feature to the first polygon
                              //  point that is in the displayed area so we'll know which feature goes with
                              //  which polygon.

                              for (uint32_t j = 0 ; j < misc->feature[i].poly_count ; j++)
                                {
                                  if (bfd_polygon.longitude[j] >= misc->displayed_area.min_x &&
                                      bfd_polygon.longitude[j] <= misc->displayed_area.max_x &&
                                      bfd_polygon.latitude[j] >= misc->displayed_area.min_y &&
                                      bfd_polygon.latitude[j] <= misc->displayed_area.max_y)
                                    {
                                      map->drawLine (misc->feature[i].longitude, misc->feature[i].latitude, bfd_polygon.longitude[j],
                                                     bfd_polygon.latitude[j], options->feature_color, 2, NVFalse,
                                                     Qt::DotLine);
                                      break;
                                    }
                                }


                              if (options->feature_poly_color.alpha () < 255 && misc->feature[i].poly_type)
                                {
                                  map->fillPolygon (misc->feature[i].poly_count, bfd_polygon.longitude, bfd_polygon.latitude,
                                                    options->feature_poly_color, NVFalse);
                                }
                              else
                                {
                                  //  If we're drawing a polygon, clip the lines.

                                  for (uint32_t j = 0 ; j < misc->feature[i].poly_count ; j++)
                                    {
                                      //  Don't close polylines (misc->feature[i].poly_type 0), only polygons.

                                      if (!misc->feature[i].poly_type && j == (misc->feature[i].poly_count - 1)) break;

                                      int32_t k = (j + 1) % misc->feature[i].poly_count;


                                      //  Don't clip the original points, clip a copy.

                                      nx[0] = bfd_polygon.longitude[j];
                                      ny[0] = bfd_polygon.latitude[j];
                                      nx[1] = bfd_polygon.longitude[k];
                                      ny[1] = bfd_polygon.latitude[k];


                                      if (clip (&nx[0], &ny[0], &nx[1], &ny[1], misc->displayed_area))
                                        {
                                          map->drawLine (nx[0], ny[0], nx[1], ny[1],
                                                         options->feature_poly_color, 2, NVFalse, Qt::SolidLine);
                                        }
                                    }
                                }
                            }
                        }


                      //  Check to see if this is a child record.

                      if (misc->feature[i].parent_record)
                        {
                          map->drawCircle (x, y, 8, 0.0, 360.0, LINE_WIDTH, tmp, Qt::SolidLine, NVFalse);
                        }
                      else
                        {
                          map->drawCircle (x, y, 12, 0.0, 360.0, LINE_WIDTH, tmp, Qt::SolidLine, NVFalse);
                        }
                    }
                }
            }
        }
    }
  map->flush ();
}
