
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



#include "mosaicViewDef.hpp"


uint32_t getColorOffset (QString colorInt)
{
  static uint32_t mult;

  if (colorInt.contains ("Alpha"))
    {
      mult = 0x1000000;
    }
  else if (colorInt.contains ("Red"))
    {
      mult = 0x10000;
    }
  else if (colorInt.contains ("Green"))
    {
      mult = 0x100;
    }
  else if (colorInt.contains ("Blue"))
    {
      mult = 0x1;
    }

  return (mult);
}



char *get_geotiff (char *mosaic_file, MISC *misc)
{
  static char string[512];

  strcpy (string, "Success");


  if (strstr (mosaic_file, ".tif") || strstr (mosaic_file, ".TIF"))
    {
      GDALDataset     *poDataset;
      double          adfGeoTransform[6];


      GDALAllRegister ();


      poDataset = (GDALDataset *) GDALOpen (mosaic_file, GA_ReadOnly);
      if (poDataset != NULL)
        {
          if (poDataset->GetProjectionRef ()  != NULL)
            {
              QString projRef = QString (poDataset->GetProjectionRef ());

              if (projRef.contains ("GEOGCS"))
                {
                  if (poDataset->GetGeoTransform (adfGeoTransform) == CE_None)
                    {
                      misc->lon_step = adfGeoTransform[1];
                      misc->lat_step = -adfGeoTransform[5];


                      misc->mosaic_width = poDataset->GetRasterXSize ();
                      misc->mosaic_height = poDataset->GetRasterYSize ();


                      misc->geotiff_area.min_x = adfGeoTransform[0];
                      misc->geotiff_area.max_y = adfGeoTransform[3];


                      misc->geotiff_area.min_y = misc->geotiff_area.max_y - misc->mosaic_height * misc->lat_step;
                      misc->geotiff_area.max_x = misc->geotiff_area.min_x + misc->mosaic_width * misc->lon_step;
                    }
                  else
                    {
                      delete poDataset;
                      sprintf (string, "File %s contains projected data", gen_basename (mosaic_file));
                      return (string);
                    }
                }
              else
                {
                  delete poDataset;
                  sprintf (string, "File %s contains Non-geographic coordinate system", gen_basename (mosaic_file));
                  return (string);
                }
            }
          else
            {
              delete poDataset;
              sprintf (string, "File %s contains no datum/projection information", gen_basename (mosaic_file));
              return (string);
            }

          if (misc->full_res_image != NULL) delete misc->full_res_image;


          misc->full_res_image = new QImage (mosaic_file);

          if (misc->full_res_image == NULL || misc->full_res_image->width () == 0 || misc->full_res_image->height () == 0)
            {
              sprintf (string, "Unable to read file %s", gen_basename (mosaic_file));
              delete poDataset;
              return (string);
            }


          /*  This is how I had to read geoTIFFs from Qt 4.7.2 until 4.8.5.  For some unknown reason the above method quit working in
              Qt 4.7.2 and I didn't check it again until !t 4.8.5 when it was again working.  I'm leaving this here for educational
              purposes.

          GDALRasterBand *poBand[4];
          QString dataType[4], colorInt[4];
          uint32_t mult[4] = {0, 0, 0, 0};


          int32_t rasterCount = poDataset->GetRasterCount ();
          if (rasterCount < 3)
            {
              delete poDataset;
              sprintf (string, "Not enough raster bands in geoTIFF");
              return (string);
            }

          for (int32_t i = 0 ; i < rasterCount ; i++)
            {
              poBand[i] = poDataset->GetRasterBand (i + 1);

              dataType[i] = QString (GDALGetDataTypeName (poBand[i]->GetRasterDataType ()));
              colorInt[i] = QString (GDALGetColorInterpretationName (poBand[i]->GetColorInterpretation ()));


              //  We can only handle Byte data (i.e. RGB or ARGB)

              if (dataType[i] != "Byte")
                {
                  delete poDataset;
                  sprintf (string, "Can only handle Byte data type");
                  return (string);
                }

              mult[i] = getColorOffset (colorInt[i]);
            }

          int32_t nXSize = poBand[0]->GetXSize ();
          int32_t nYSize = poBand[0]->GetYSize ();

          misc->full_res_image = new QImage (nXSize, nYSize, QImage::Format_ARGB32);
          if (misc->full_res_image == NULL || misc->full_res_image->width () == 0 || misc->full_res_image->height () == 0)
            {
              sprintf (string, "Unable to open image!");
              delete poDataset;
              return (string);
            }

          uint32_t *color = new uint32_t[nXSize];
          uint8_t *pafScanline = (uint8_t *) CPLMalloc (sizeof (uint8_t) * nXSize);

          for (int32_t i = 0 ; i < nYSize ; i++)
            {
              //  If we don't have an alpha band set it to 255.

              for (int32_t k = 0 ; k < nXSize ; k++)
                {
                  if (rasterCount < 4)
                    {
                      color[k] = 0xff000000;
                    }
                  else
                    {
                      color[k] = 0x0;
                    }
                }


              //  Read the raster bands.

              for (int32_t j = 0 ; j < rasterCount ; j++)
                {
                  poBand[j]->RasterIO (GF_Read, 0, i, nXSize, 1, pafScanline, nXSize, 1, GDT_Byte, 0, 0);
                  for (int32_t k = 0 ; k < nXSize ; k++) color[k] += ((uint32_t) pafScanline[k]) * mult[j];
                }


              //  Set the image pixels.

              for (int32_t k = 0 ; k < nXSize ; k++)
                {
                  misc->full_res_image->setPixel (k, i, color[k]);
                }
            }

          delete (color);
          CPLFree (pafScanline);
          */


          delete poDataset;
        }
      else
        {
          sprintf (string, "Unable to open file %s", gen_basename (mosaic_file));
          return (string);
        }
    }
  else
    {
      sprintf (string, "File %s is not a GeoTIFF file", gen_basename (mosaic_file));
      return (string);
    }


  return (string);
}
