/***************************************************************************
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 *         Title:                                                          *
 *   Description:                                                          *
 *                                                                         *
 *                                                                         *
 *  Input:                                                                 *
 *   fname    - string with file name of image to decode                   *
 *   page_num - the number of the page to decode, 0 if ommited             *
 *              in time series pages are time points                       *
 *              in the z series pages are depth points                     *
 *                                                                         *
 *  Output:                                                                *
 *   im       - matrix of the image in the native format with channels     *
 *              in the 3d dimension                                        *
 *   format   - string with format name used to decode the image           *
 *   pages    - number of pages in the image                               *
 *   xyzres   - pixel size on x,y,z dim in microns double[3]               *
 *   metatxt  - string with all meta-data extracted from the image         *
 *                                                                         *
 *  ex:                                                                    *
 *   [im, format, pages, xyzr, metatxt] = bimreadh5(fname, page_num);      *
 *                                                                         *
 *   [im] = bimreadh5(fname);                                              *
 *                                                                         *
 *                                                                         *
 ***************************************************************************/
// boost.Python includes
#include "boost/python/module.hpp"
#include "boost/python/def.hpp"
#include "boost/python/object.hpp"
#include "boost/python/tuple.hpp"

// our interface to imgcnv includes:
#include <bimreadh5.hh>

// imgcnv includes:
#include <xstring.h>

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <string.h>
#include <stdio.h>



/********************************************************************
 * filename        
 * page            
 ********************************************************************/
boost::python::object boost_bimreadh5(const char* aFileName, int aPageNumber=0)
{
  /********************************************************************
   * Input Parameter Checking
   ********************************************************************/

  // output
  std::string fmt = "";
  int npage=0;
  double vPixelSize[3] = {0.0, 0.0, 0.0};

  fprintf(stdout, "Image: %s, Page %d\n", aFileName, aPageNumber);

  //-----------------------------------------------------------
  // now read image and metadata
  //-----------------------------------------------------------

  bim::MetaFormatManager vMetaFormatManager;
  bim::Image vImage;

  if (vMetaFormatManager.sessionStartRead((bim::Filename)aFileName) != 0)
    fprintf(stdout, "Input format is not supported.\n");

  npage = vMetaFormatManager.sessionGetNumberOfPages();
  fmt = vMetaFormatManager.sessionGetFormatName();

  if (aPageNumber<0) {
    aPageNumber=0;
    fprintf(stdout, "Requested page number is invalid, used %d.\n", aPageNumber);
  }

  if (aPageNumber>=npage) {
    aPageNumber=npage-1;
    fprintf(stdout, "Requested page number is invalid, used %d.\n", aPageNumber);
  }

  if (vMetaFormatManager.sessionReadImage(vImage.imageBitmap(), aPageNumber) != 0) {
    fprintf(stdout, "Problems reading the image\n");
  }

  // getting metadata fields
  vMetaFormatManager.sessionParseMetaData(aPageNumber);
  vPixelSize[0] = vMetaFormatManager.getPixelSizeX();
  vPixelSize[1] = vMetaFormatManager.getPixelSizeY();
  vPixelSize[2] = vMetaFormatManager.getPixelSizeZ();

#if defined (DEBUG) || defined (_DEBUG)
  fprintf(stdout, "Pixel resolution: %.8f, %.8f, %.8f\n", vPixelSize[0], vPixelSize[1], vPixelSize[2]);
#endif
  fprintf(stdout, "Pixel resolution: %.8f, %.8f, %.8f\n", vPixelSize[0], vPixelSize[1], vPixelSize[2]);

  // get meta text if required
  //bim::TagMap vMetaDataMap;
  //if (nlhs > 4) {
  //  vMetaDataMap = vMetaFormatManager.get_metadata();
  //}

  vMetaFormatManager.sessionEnd();


  //-----------------------------------------------------------
  // pre-poc input image
  //-----------------------------------------------------------

  // make sure red image is in supported pixel format, e.g. will convert 12 bit to 16 bit
  vImage = vImage.ensureTypedDepth();

  //-----------------------------------------------------------
  // create output image
  //-----------------------------------------------------------
/*
#if defined(MX_API_VER) && (MX_API_VER > 0x07000000)
  const mwSize dims[] = { vImage.height(), vImage.width(), vImage.samples() };
#else
  const int dims[] = { vImage.height(), vImage.width(), vImage.samples() };
#endif
*/

  size_t size  = vImage.samples() * vImage.width() * vImage.height();
  bim::uint8 *plhs = new bim::uint8[ size * vImage.depth()/8 ];
  // UNSIGNED
  if      (vImage.depth()==8  && vImage.pixelType()==bim::FMT_UNSIGNED) { copy_data<bim::uint8> (vImage, (bim::uint8*) plhs); }
  else if (vImage.depth()==16 && vImage.pixelType()==bim::FMT_UNSIGNED) { copy_data<bim::uint16>(vImage, (bim::uint16*)plhs); }
  else if (vImage.depth()==32 && vImage.pixelType()==bim::FMT_UNSIGNED) { copy_data<bim::uint32>(vImage, (bim::uint32*)plhs); }
  else // SIGNED
  if      (vImage.depth()==8  && vImage.pixelType()==bim::FMT_SIGNED)   { copy_data<bim::int8>  (vImage, (bim::int8*)  plhs); }
  else if (vImage.depth()==16 && vImage.pixelType()==bim::FMT_SIGNED)   { copy_data<bim::int16> (vImage, (bim::int16*) plhs); }
  else if (vImage.depth()==32 && vImage.pixelType()==bim::FMT_SIGNED)   { copy_data<bim::int32> (vImage, (bim::int32*) plhs); }
  else // FLOAT
  if      (vImage.depth()==32 && vImage.pixelType()==bim::FMT_FLOAT)    { copy_data<float>      (vImage, (float*)      plhs); }
  else if (vImage.depth()==64 && vImage.pixelType()==bim::FMT_FLOAT)    { copy_data<double>     (vImage, (double*)     plhs); }

/*
  //-----------------------------------------------------------
  // create output meta-data
  //-----------------------------------------------------------

  // if need to return format
  if (nlhs > 1) {
    char *output_buf = (char*) mxCalloc(vFormatName.size()+1, sizeof(char));
    output_buf[vFormatName.size()] = 0;
    memcpy(output_buf, vFormatName.c_str(), vFormatName.size());
    plhs[1] = mxCreateString(output_buf);
  }

  // if need to return aNumPages
  if (nlhs > 2) {
    plhs[2] = mxCreateDoubleMatrix(1, 1, mxREAL);
    double *v = mxGetPr(plhs[2]);
    *v = aNumPages;
  }

  // if need to return xyzres
  if (nlhs > 3) {
    plhs[3] = mxCreateDoubleMatrix(3, 1, mxREAL);
    double *v = mxGetPr(plhs[3]);
    v[0] = vPixelSize[0];
    v[1] = vPixelSize[1];
    v[2] = vPixelSize[2];
  }

  // if need to return meta data
  if (nlhs > 4) {
    mwIndex subs[2];
    subs[0] = 0;
    subs[1] = 0;

    mwSize dims[2];
    dims[0] = vMetaDataMap.size()+1;
    dims[1] = 2;
    plhs[4] = mxCreateCellArray(2, dims);

    // first add the filename
    subs[1] = 0; mxSetCell(plhs[4], mxCalcSingleSubscript(plhs[4], 2, subs), mxCreateString("Filename"));
    subs[1] = 1; mxSetCell(plhs[4], mxCalcSingleSubscript(plhs[4], 2, subs), mxCreateString(fbasename.c_str()));
    subs[0] = subs[0] + 1;

    // then add the rest of the map:
    std::map<std::string, std::string>::const_iterator vMapIt;
    for (vMapIt = vMetaDataMap.begin(); vMapIt != vMetaDataMap.end(); ++vMapIt) {
      subs[1] = 0; mxSetCell(plhs[4], mxCalcSingleSubscript(plhs[4], 2, subs), mxCreateString(vMapIt->first.c_str()));
      subs[1] = 1; mxSetCell(plhs[4], mxCalcSingleSubscript(plhs[4], 2, subs), mxCreateString(vMapIt->second.c_str()));
      subs[0] = subs[0] + 1;
    }
  }
*/

  boost::python::object bmp;
  size_t osize  = vImage.samples() * vImage.width() * vImage.height();
  // UNSIGNED
  if      (vImage.depth()==8  && vImage.pixelType()==bim::FMT_UNSIGNED)
      bmp = boost::python::object(boost::python::handle<>(boost::python::borrowed (PyBuffer_FromMemory ((bim::uint8 *) &plhs, osize))));
  else if (vImage.depth()==16 && vImage.pixelType()==bim::FMT_UNSIGNED)
      bmp = boost::python::object(boost::python::handle<>(boost::python::borrowed (PyBuffer_FromMemory ((bim::uint16 *) &plhs, osize))));
  else if (vImage.depth()==32 && vImage.pixelType()==bim::FMT_UNSIGNED)
      bmp = boost::python::object(boost::python::handle<>(boost::python::borrowed (PyBuffer_FromMemory ((bim::uint32 *) &plhs, osize))));
  else // SIGNED
  if      (vImage.depth()==8  && vImage.pixelType()==bim::FMT_SIGNED)
      bmp = boost::python::object(boost::python::handle<>(boost::python::borrowed (PyBuffer_FromMemory ((bim::int8 *) &plhs, osize))));
  else if (vImage.depth()==16 && vImage.pixelType()==bim::FMT_SIGNED)
      bmp = boost::python::object(boost::python::handle<>(boost::python::borrowed (PyBuffer_FromMemory ((bim::int16 *) &plhs, osize))));
  else if (vImage.depth()==32 && vImage.pixelType()==bim::FMT_SIGNED)
      bmp = boost::python::object(boost::python::handle<>(boost::python::borrowed (PyBuffer_FromMemory ((bim::int32 *) &plhs, osize))));
  else // FLOAT
  if      (vImage.depth()==32 && vImage.pixelType()==bim::FMT_FLOAT) 
      bmp = boost::python::object(boost::python::handle<>(boost::python::borrowed (PyBuffer_FromMemory ((float *) &plhs, osize))));
  else if (vImage.depth()==64 && vImage.pixelType()==bim::FMT_FLOAT)
      bmp = boost::python::object(boost::python::handle<>(boost::python::borrowed (PyBuffer_FromMemory ((float *) &plhs, osize))));

  return make_tuple(
      bmp,
      fmt,
      npage,
      vPixelSize[0], vPixelSize[1], vPixelSize[2],
      "dummy_metadata");
}


BOOST_PYTHON_MODULE(libbimreadh5)
{
    def("bimreadh5", boost_bimreadh5);
}

