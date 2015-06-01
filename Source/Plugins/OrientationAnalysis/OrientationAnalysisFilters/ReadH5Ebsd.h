/* ============================================================================
* Copyright (c) 2009-2015 BlueQuartz Software, LLC
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
* Neither the name of BlueQuartz Software, the US Air Force, nor the names of its
* contributors may be used to endorse or promote products derived from this software
* without specific prior written permission.
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
*
* The code contained herein was partially funded by the followig contracts:
*    United States Air Force Prime Contract FA8650-07-D-5800
*    United States Air Force Prime Contract FA8650-10-D-5210
*    United States Prime Contract Navy N00173-07-C-2068
*
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


#ifndef _ReadH5Ebsd_H_
#define _ReadH5Ebsd_H_

#include "DREAM3DLib/DREAM3DLib.h"
#include "DREAM3DLib/Common/AbstractFilter.h"
#include "DREAM3DLib/Common/DREAM3DSetGetMacros.h"
#include "DREAM3DLib/DataArrays/StringDataArray.hpp"

#include "EbsdLib/H5EbsdVolumeReader.h"

class H5EbsdVolumeReader;

/**
 * @brief The ReadH5Ebsd class. See [Filter documentation](@ref readh5ebsd) for details.
 */
class ReadH5Ebsd : public AbstractFilter
{
    Q_OBJECT /* Need this for Qt's signals and slots mechanism to work */
  public:
    DREAM3D_SHARED_POINTERS(ReadH5Ebsd)
    DREAM3D_STATIC_NEW_MACRO(ReadH5Ebsd)
    DREAM3D_TYPE_MACRO_SUPER(ReadH5Ebsd, AbstractFilter)

    virtual ~ReadH5Ebsd();

    DREAM3D_FILTER_PARAMETER(QString, DataContainerName)
    Q_PROPERTY(QString DataContainerName READ getDataContainerName WRITE setDataContainerName)

    DREAM3D_FILTER_PARAMETER(QString, CellEnsembleAttributeMatrixName)
    Q_PROPERTY(QString CellEnsembleAttributeMatrixName READ getCellEnsembleAttributeMatrixName WRITE setCellEnsembleAttributeMatrixName)

    DREAM3D_FILTER_PARAMETER(QString, CellAttributeMatrixName)
    Q_PROPERTY(QString CellAttributeMatrixName READ getCellAttributeMatrixName WRITE setCellAttributeMatrixName)

    DREAM3D_INSTANCE_STRING_PROPERTY(PhaseNameArrayName)

    DREAM3D_INSTANCE_STRING_PROPERTY(MaterialNameArrayName)

    DREAM3D_FILTER_PARAMETER(QString, InputFile)
    Q_PROPERTY(QString InputFile READ getInputFile WRITE setInputFile)

    DREAM3D_FILTER_PARAMETER(int, ZStartIndex)
    Q_PROPERTY(int ZStartIndex READ getZStartIndex WRITE setZStartIndex)

    DREAM3D_FILTER_PARAMETER(int, ZEndIndex)
    Q_PROPERTY(int ZEndIndex READ getZEndIndex WRITE setZEndIndex)

    DREAM3D_FILTER_PARAMETER(bool, UseTransformations)
    Q_PROPERTY(bool UseTransformations READ getUseTransformations WRITE setUseTransformations)

    DREAM3D_INSTANCE_PROPERTY(QSet<QString>, SelectedArrayNames)
    Q_PROPERTY(QSet<QString> SelectedArrayNames READ getSelectedArrayNames WRITE setSelectedArrayNames)

    DREAM3D_INSTANCE_PROPERTY(QSet<QString>, DataArrayNames) // These are for reading the names of the arrays during a preflight
    Q_PROPERTY(QSet<QString> DataArrayNames READ getDataArrayNames WRITE setDataArrayNames)

    //-------------------------------------------------------
    // Not sure why these are here. We would be reading all of these from the file
    //
    DREAM3D_INSTANCE_PROPERTY(uint32_t, RefFrameZDir)
    DREAM3D_INSTANCE_PROPERTY(int32_t, Manufacturer)
    DREAM3D_INSTANCE_PROPERTY(AxisAngleInput_t, SampleTransformation)
    DREAM3D_INSTANCE_PROPERTY(AxisAngleInput_t, EulerTransformation)
    //-------------------------------------------------------

    virtual const QString getCompiledLibraryName();
    virtual AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters);
    virtual const QString getGroupName();
    virtual const QString getSubGroupName();
    virtual const QString getHumanLabel();

    virtual void setupFilterParameters();

    /**
    * @brief This method will write the options to a file
    * @param writer The writer that is used to write the options to a file
    */
    virtual int writeFilterParameters(AbstractFilterParametersWriter* writer, int index);

    /**
    * @brief This method will read the options from a file
    * @param reader The reader that is used to read the options from a file
    */
    virtual void readFilterParameters(AbstractFilterParametersReader* reader, int index);

    /**
     * @brief Reimplemented from @see AbstractFilter class
     */
    virtual void execute();
    virtual void preflight();

  signals:
    void updateFilterParameters(AbstractFilter* filter);
    void parametersChanged();
    void preflightAboutToExecute();
    void preflightExecuted();

  protected:
    ReadH5Ebsd();

    void dataCheck();

    /**
     * @brief initDataContainerDimsRes Initializes the dimensions to the correct size
     * @param dims Data container dimensions
     * @param m DataContainer instance pointer
     * @return Integer error value
     */
    int32_t initDataContainerDimsRes(int64_t dims[3], DataContainer::Pointer m);

    /**
     * @brief readVolumeInfo Reads the dimension information
     */
    void readVolumeInfo();

    /**
     * @brief initTSLEbsdVolumeReader Initializes the H5EbsdVolumeReader to a H5AngReader pointer
     * @return H5EbsdVolumeReader instance pointer
     */
    H5EbsdVolumeReader::Pointer initTSLEbsdVolumeReader();

    /**
     * @brief initHKLEbsdVolumeReader Initializes the H5EbsdVolumeReader to a H5CtfVolumeReader pointer
     * @return H5EbsdVolumeReader instance pointer
     */
    H5EbsdVolumeReader::Pointer initHKLEbsdVolumeReader();

    /**
     * @brief copyTSLArrays Copies the read arrays into the data container structure (TSL variant)
     * @param ebsdReader H5EbsdVolumeReader instance pointer
     */
    void copyTSLArrays(H5EbsdVolumeReader* ebsdReader);

    /**
     * @brief copyHKLArrays Copies the read arrays into the data container structure (HKL variant)
     * @param ebsdReader H5EbsdVolumeReader instance pointer
     */
    void copyHKLArrays(H5EbsdVolumeReader* ebsdReader);

    /**
    * @brief loadInfo Reads the values for the phase type, crystal structure
    * and precipitate fractions from the EBSD file.
    * @param reader EbsdReader instance pointer
    * @return Integer error value
    */
    template<typename EbsdReader, typename EbsdPhase>
    int32_t loadInfo(EbsdReader* reader)
    {
      DataContainer::Pointer vdc = getDataContainerArray()->getDataContainer(getDataContainerName());
      if(NULL == vdc) { return -1;}
      AttributeMatrix::Pointer attrMatrix = vdc->getAttributeMatrix(getCellEnsembleAttributeMatrixName() );
      if(NULL == attrMatrix.get() ) { return -2; }

      reader->setFileName(m_InputFile);
      reader->setSliceStart(m_ZStartIndex);
      reader->setSliceEnd(m_ZEndIndex);

      QVector<typename EbsdPhase::Pointer> phases = reader->getPhases();
      if (phases.size() == 0)
      {
        setErrorCondition(reader->getErrorCode());
        notifyErrorMessage(getHumanLabel(), reader->getErrorMessage(), getErrorCondition());
        return getErrorCondition();
      }

      // Resize the Ensemble Attribute Matrix to be the correct number of phases.
      QVector<size_t> tDims(1, phases.size() + 1);
      attrMatrix->resizeAttributeArrays(tDims);

      DataArray<uint32_t>::Pointer crystalStructures = DataArray<uint32_t>::CreateArray(phases.size() + 1, getCrystalStructuresArrayName());
      StringDataArray::Pointer materialNames = StringDataArray::CreateArray(phases.size() + 1, getMaterialNameArrayName());
      QVector<size_t> cDims(1, 6);
      FloatArrayType::Pointer latticeConstants = FloatArrayType::CreateArray(phases.size() + 1, cDims, getLatticeConstantsArrayName());

      // Initialize the zero'th element to unknowns. The other elements will
      // be filled in based on values from the data file
      crystalStructures->setValue(0, Ebsd::CrystalStructure::UnknownCrystalStructure);
      materialNames->setValue(0, "Invalid Phase");
      latticeConstants->setComponent(0, 0, 0.0f);
      latticeConstants->setComponent(0, 1, 0.0f);
      latticeConstants->setComponent(0, 2, 0.0f);
      latticeConstants->setComponent(0, 3, 0.0f);
      latticeConstants->setComponent(0, 4, 0.0f);
      latticeConstants->setComponent(0, 5, 0.0f);

      for (size_t i = 0; i < phases.size(); i++)
      {
        int32_t phaseID = phases[i]->getPhaseIndex();
        crystalStructures->setValue(phaseID, phases[i]->determineCrystalStructure() );
        materialNames->setValue(phaseID, phases[i]->getMaterialName());
        QVector<float> lc = phases[i]->getLatticeConstants();

        latticeConstants->setComponent(phaseID, 0, lc[0]);
        latticeConstants->setComponent(phaseID, 1, lc[1]);
        latticeConstants->setComponent(phaseID, 2, lc[2]);
        latticeConstants->setComponent(phaseID, 3, lc[3]);
        latticeConstants->setComponent(phaseID, 4, lc[4]);
        latticeConstants->setComponent(phaseID, 5, lc[5]);
      }


      attrMatrix->addAttributeArray(DREAM3D::EnsembleData::CrystalStructures, crystalStructures);
      m_CrystalStructuresPtr = crystalStructures;
      m_CrystalStructures = crystalStructures->getPointer(0);
      attrMatrix->addAttributeArray(DREAM3D::EnsembleData::LatticeConstants, latticeConstants);
      m_LatticeConstantsPtr = latticeConstants;
      m_LatticeConstants = latticeConstants->getPointer(0);
      attrMatrix->addAttributeArray(DREAM3D::EnsembleData::MaterialName, materialNames);
      m_MaterialNamesPtr = materialNames;
      return 0;
    }

  private:
    DEFINE_REQUIRED_DATAARRAY_VARIABLE(int32_t, CellPhases)
    DEFINE_REQUIRED_DATAARRAY_VARIABLE(float, CellEulerAngles)
    DEFINE_REQUIRED_DATAARRAY_VARIABLE(uint32_t, CrystalStructures)
    DEFINE_REQUIRED_DATAARRAY_VARIABLE(float, LatticeConstants)

    StringDataArray::WeakPointer  m_MaterialNamesPtr;

    ReadH5Ebsd(const ReadH5Ebsd&); // Copy Constructor Not Implemented
    void operator=(const ReadH5Ebsd&); // Operator '=' Not Implemented
};

#endif /* ReadH5Ebsd_H_ */
