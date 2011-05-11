/* ============================================================================
 * Copyright (c) 2010, Michael A. Jackson (BlueQuartz Software)
 * Copyright (c) 2010, Dr. Michael A. Grober (US Air Force Research Laboratories
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the name of Michael A. Jackson nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "SurfaceMesh.h"
#include "SMVtkFileIO.h"

#include <MXA/Common/LogTime.h>
#include <MXA/Utilities/MXADir.h>



#ifdef AIM_USE_QT
#define CHECK_FOR_CANCELED(AClass)\
  if (this->m_Cancel) { \
  QString msg = #AClass; \
  msg += " was Canceled"; \
  emit updateMessage(msg);\
  emit updateProgress(0);\
  return;}

#else
#define CHECK_FOR_CANCELED(AClass)\
  ;
#endif



#if AIM_USE_QT

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
SurfaceMesh::Pointer SurfaceMesh::New( QObject* parent)
{
  Pointer sharedPtr(new SurfaceMesh(parent));
  return sharedPtr;
}
#endif

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
SurfaceMesh::SurfaceMesh(
#if AIM_USE_QT
QObject* parent
#endif
) :
#if AIM_USE_QT
QObject(parent),
#endif
m_InputDirectory("."),
m_InputFile(""),
m_ScalarName(AIM::Reconstruction::GrainIdScalarName),
m_OutputDirectory(""),
m_OutputFilePrefix("SurfaceMesh_"),
m_ConformalMesh(true),
m_BinaryVTKFile(false),
m_DeleteTempFiles(true),
m_SmoothMesh(false),
m_SmoothIterations(0),
m_SmoothFileOutputIncrement(0),
m_SmoothLockQuadPoints(false),
m_ErrorCondition(0)
#if AIM_USE_QT
,
m_Cancel(false)
#endif
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
SurfaceMesh::~SurfaceMesh()
{
}

#define MAKE_OUTPUT_FILE_PATH(outpath, filename)\
    std::string outpath = m_OutputDirectory + MXADir::Separator + m_OutputFilePrefix + filename;

#define USE_VTK_FILE_UTILS 1

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SurfaceMesh::compute()
{
  CHECK_FOR_CANCELED(Surface Meshing)
  progressMessage(AIM_STRING("Running Surface Meshing"), 0 );

//  MAKE_OUTPUT_FILE_PATH (  NodesRawFile , AIM::SurfaceMeshing::NodesRawFile)
  MAKE_OUTPUT_FILE_PATH (  NodesFile , AIM::SurfaceMeshing::NodesFileBin)
  MAKE_OUTPUT_FILE_PATH (  TrianglesFile , AIM::SurfaceMeshing::TrianglesFileBin)
//  MAKE_OUTPUT_FILE_PATH (  EdgesFile , AIM::SurfaceMeshing::EdgesFile)
//  MAKE_OUTPUT_FILE_PATH (  EdgesFileIndex , AIM::SurfaceMeshing::EdgesFileIndex)
//  MAKE_OUTPUT_FILE_PATH (  TrianglesFileIndex , AIM::SurfaceMeshing::TrianglesFileIndex)
  MAKE_OUTPUT_FILE_PATH (  VisualizationFile , AIM::SurfaceMeshing::VisualizationFile)

  // Create the output directory if needed
  if (MXADir::exists(m_OutputDirectory) == false)
  {
    if (MXADir::mkdir(m_OutputDirectory, true) == false)
    {
#ifdef AIM_USE_QT
    this->m_Cancel = true;
#endif
    this->m_ErrorCondition = 1;
    CHECK_FOR_CANCELED(Surface Meshing)
    progressMessage(AIM_STRING("Could not create output directory."), 100 );
    return;
    }
  }
  int err = 0;
  int cNodeID = 0;
//  int cEdgeID = 0;
//  int fEdgeID = 0;
  int cTriID = 0;
//  int nFEdge = 0; // number of edges on the square...
  int nTriangle = 0; // number of triangles...
  int nEdge = 0; // number of triangles...
//  int nnEdge = 0; // number of triangles...
//  int npTriangle = 0; // number of triangles...
//  int ncTriangle = 0; // number of triangles...
  int nNodes = 0; // number of total nodes used...
  int edgeTable_2d[20][8] = {
  { -1, -1, -1, -1, -1, -1, -1, -1},
  { -1, -1, -1, -1, -1, -1, -1, -1},
  { -1, -1, -1, -1, -1, -1, -1, -1},
  { 0, 1, -1, -1, -1, -1, -1, -1},
  { -1, -1, -1, -1, -1, -1, -1, -1},
  { 0, 2, -1, -1, -1, -1, -1, -1},
  { 1, 2, -1, -1, -1, -1, -1, -1},
  { 0, 4, 2, 4, 1, 4, -1, -1},
  { -1, -1, -1, -1, -1, -1, -1, -1},
  { 3, 0, -1, -1, -1, -1, -1, -1},
  { 3, 1, -1, -1, -1, -1, -1, -1},
  { 3, 4, 0, 4, 1, 4, -1, -1},
  { 2, 3, -1, -1, -1, -1, -1, -1},
  { 3, 4, 0, 4, 2, 4, -1, -1},
  { 3, 4, 1, 4, 2, 4, -1, -1},
  { 3, 0, 1, 2, -1, -1, -1, -1},
  { 0, 1, 2, 3, -1, -1, -1, -1},
  { 0, 1, 2, 3, -1, -1, -1, -1},
  { 3, 0, 1, 2, -1, -1, -1, -1},
  { 3, 4, 1, 4, 0, 4, 2, 4}
  };

  int nsTable_2d[20][8] = {
    {-1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1},
    {1, 0, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1 },
    {1, 0, -1, -1, -1, -1, -1, -1, },
    {2, 1, -1, -1, -1, -1, -1, -1},
    {1, 0, 3, 2, 2, 1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1},
    {0, 3, -1, -1, -1, -1, -1, -1},
    {0, 3, -1, -1, -1, -1, -1, -1},
    {0, 3, 1, 0, 2, 1, -1, -1},
    {3, 2, -1, -1, -1, -1, -1, -1},
    {0, 3, 1, 0, 3, 2, -1, -1},
    {0, 3, 2, 1, 3, 2, -1, -1},
    {0, 3, 2, 1, -1, -1, -1, -1},
    {1, 0, 3, 2, -1, -1, -1, -1},
    {1, 0, 3, 2, -1, -1, -1, -1},
    {0, 3, 2, 1, -1, -1, -1, -1},
    {0, 3, 2, 1, 1,  0, 3, 2 }
  };


  m = SurfaceMeshFunc::New();
//  int err = 0;
#if  USE_VTK_FILE_UTILS
  SMVtkFileIO vtkreader;
  m_ZDim = vtkreader.primeFileToScalarDataLocation(m.get(), m_InputFile, m_ScalarName);

#else
  m_ZDim = m->initialize_micro(m_InputFile, -1);
#endif
  std::stringstream ss;
  for (int i = 0; i < (m_ZDim - 1); i++)
  {
    ss.str("");
    ss << "Marching Cubes Between Layers " << i << " and " << i+1;
    progressMessage(AIM_STRING(ss.str().c_str()), (i*90/m_ZDim) );

    // initialize neighbors, possible nodes and squares of marching cubes of each layer...
#if  USE_VTK_FILE_UTILS
    m_ZDim = vtkreader.readZSlice(m.get(), i);
    if(m_ZDim == -1)
    {
      ss.str("");
      ss << "Error loading slice data from vtk file.";
      this->m_ErrorCondition = 1;
      progressMessage(AIM_STRING(ss.str().c_str()), 100 );
      return;
    }
#else
    m_ZDim = m->initialize_micro(m_InputFile, i);
#endif
    m->get_neighbor_list(i);
    m->initialize_nodes(i);
    m->initialize_squares(i);

    // find face edges of each square of marching cubes in each layer...
    nEdge = m->get_number_Edges(i);
    m->get_nodes_Edges(edgeTable_2d, nsTable_2d, i, nEdge);

    // find triangles and arrange the spins across each triangle...
    nTriangle = m->get_number_triangles();

    if (nTriangle > 0)
    {
      m->get_triangles(nTriangle);
      m->arrange_grainnames(nTriangle, i);
    }

    // assign new, cumulative node id...
    nNodes = m->assign_nodeID(cNodeID, i);
    // std::cout << "nNodes: " << nNodes << std::endl;
    // Output nodes and triangles...
    m->writeNodesFile(i, cNodeID, NodesFile);
    err = m->writeTrianglesFile(nTriangle, TrianglesFile, i, cTriID);
    if (err < 0)
    {
      this->m_Cancel = true;
      progressMessage(AIM_STRING("Error Writing Triangles Temp File"), 100 );
#if AIM_USE_QT
  emit finished();
#endif
      return;
    }
    cNodeID = nNodes;
    cTriID = cTriID + nTriangle;
  }


  if (m_SmoothMesh) {
    progressMessage(AIM_STRING("Smoothing Boundaries"), 90 );
    m->smooth_boundaries(nNodes, cTriID, NodesFile, TrianglesFile);
  }


#ifdef AIM_USE_QT
  QString msg("Writing Surface Mesh File: ");
  msg.append(QString::fromStdString(VisualizationFile));
#else
  std::string msg("Writing Surface Mesh File: ");
  msg.append(VisualizationFile);
#endif


  progressMessage(msg, 95 );
  SMVtkFileIO writer;
  writer.writeVTKFile(m.get(), nNodes, cTriID, VisualizationFile, NodesFile, TrianglesFile, m_BinaryVTKFile, m_ConformalMesh);

  progressMessage(AIM_STRING("Surface Meshing Complete"), 100 );

  m = SurfaceMeshFunc::NullPointer();  // Clean up the memory

  if (m_DeleteTempFiles == true)
  {
    // Delete the intermediate files
    MXADir::remove(NodesFile);
    MXADir::remove(TrianglesFile);
  }


#if AIM_USE_QT
  emit finished();
#endif
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SurfaceMesh::progressMessage(AIM_STRING message, int progress)
{
#ifdef AIM_USE_QT
  emit updateMessage(QString(message));
  emit updateProgress(progress);
  //  std::cout << message.toStdString() << std::endl;
#else
  std::cout << logTime() << progress << "% - "<< message << std::endl;
#endif
}

#ifdef AIM_USE_QT
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SurfaceMesh::on_CancelWorker()
{
  // std::cout << "SurfaceMesh::cancelWorker()" << std::endl;
  this->m_Cancel = true;
}
#endif

