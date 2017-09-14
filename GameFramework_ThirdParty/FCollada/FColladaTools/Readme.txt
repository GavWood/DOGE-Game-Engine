Package Contents:
--------------------

External\*
  These are external, open-source libraries.
  Please see the specific folders for the copyright
  and licensing information on each external library.

FCollada\*
  This is an empty folder. You will need to download
  the static library version of FCollada and unzip it in this
  folder for the samples to compile.

FCTools\*
  This is the repository for the sample COLLADA conditioners
  written using the FCollada library.

  Unless noted otherwise in the source code, these samples are
  Copyright (c) 2006 Feeling Software Inc. and released under the
  MIT license: http://www.opensource.org/licenses/mit-license.php

FCTools\FCValidate
  This is an extremely simple command-line tool that will open
  a COLLADA document and output the errors/warnings that FCollada
  found.

  While it is simple: it makes for a great starting point
  for the more complex conditioners that you will be writing.

FCTools\FCProcessImages
  This conditioner processes all the images in a COLLADA document:
  - Removes duplicate <image> instances in the image library.
  - Opens the image files, ensures that they are power-of-2 images,
    generates their mip-maps and writes them out as DDS image files.

FCTools\FCProcessMeshes
  This condition processes all the geometries in a COLLADA document:
  - Triangulates them for faster renders in the Feeling Viewer.
  - Generates texture tangents and binormals for the first texture
    coordinate channels.