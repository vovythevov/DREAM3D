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


#ifndef _WritePoleFigure_H_
#define _WritePoleFigure_H_

#include "DREAM3DLib/DREAM3DLib.h"
#include "DREAM3DLib/Common/AbstractFilter.h"
#include "DREAM3DLib/Common/DREAM3DSetGetMacros.h"

/**
 * @brief The WritePoleFigure class. See [Filter documentation](@ref writeipfstandardtriangle) for details.
 */
class WritePoleFigure : public AbstractFilter
{
    Q_OBJECT /* Need this for Qt's signals and slots mechanism to work */
  public:
    DREAM3D_SHARED_POINTERS(WritePoleFigure)
    DREAM3D_STATIC_NEW_MACRO(WritePoleFigure)
    DREAM3D_TYPE_MACRO_SUPER(WritePoleFigure, AbstractFilter)

    virtual ~WritePoleFigure();

    DREAM3D_FILTER_PARAMETER(QString, ImagePrefix)
    Q_PROPERTY(QString ImagePrefix READ getImagePrefix WRITE setImagePrefix)

    DREAM3D_FILTER_PARAMETER(QString, OutputPath)
    Q_PROPERTY(QString OutputPath READ getOutputPath WRITE setOutputPath)

    DREAM3D_FILTER_PARAMETER(int, ImageFormat)
    Q_PROPERTY(int ImageFormat READ getImageFormat WRITE setImageFormat)

    DREAM3D_FILTER_PARAMETER(int, ImageSize)
    Q_PROPERTY(int ImageSize READ getImageSize WRITE setImageSize)

    DREAM3D_FILTER_PARAMETER(int, LambertSize)
    Q_PROPERTY(int LambertSize READ getLambertSize WRITE setLambertSize)

    DREAM3D_FILTER_PARAMETER(int, NumColors)
    Q_PROPERTY(int NumColors READ getNumColors WRITE setNumColors)

    DREAM3D_FILTER_PARAMETER(int, ImageLayout)
    Q_PROPERTY(int ImageLayout READ getImageLayout WRITE setImageLayout)

    enum ImageFormatType
    {
      TifImageType = 0,
      BmpImageType = 1,
      PngImageType = 2,
      JpgImageType = 3
    };


    DREAM3D_FILTER_PARAMETER(DataArrayPath, CellEulerAnglesArrayPath)
    Q_PROPERTY(DataArrayPath CellEulerAnglesArrayPath READ getCellEulerAnglesArrayPath WRITE setCellEulerAnglesArrayPath)

    DREAM3D_FILTER_PARAMETER(DataArrayPath, CellPhasesArrayPath)
    Q_PROPERTY(DataArrayPath CellPhasesArrayPath READ getCellPhasesArrayPath WRITE setCellPhasesArrayPath)

    DREAM3D_FILTER_PARAMETER(DataArrayPath, CrystalStructuresArrayPath)
    Q_PROPERTY(DataArrayPath CrystalStructuresArrayPath READ getCrystalStructuresArrayPath WRITE setCrystalStructuresArrayPath)

    DREAM3D_FILTER_PARAMETER(DataArrayPath, GoodVoxelsArrayPath)
    Q_PROPERTY(DataArrayPath GoodVoxelsArrayPath READ getGoodVoxelsArrayPath WRITE setGoodVoxelsArrayPath)

    virtual const QString getCompiledLibraryName();
    virtual AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters);
    virtual const QString getGroupName();

    /**
     * @brief getSubGroupName This returns the subgroup within the main group for this filter.
     * @return
     */
    virtual const QString getSubGroupName();

    /**
    * @brief This returns a string that is displayed in the GUI. It should be readable
    * and understandable by humans.
    */
    virtual const QString getHumanLabel();

    /**
    * @brief This method will instantiate all the end user settable options/parameters
    * for this filter
    */
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

    /**
    * @brief This function runs some sanity checks on the DataContainer and inputs
    * in an attempt to ensure the filter can process the inputs.
    */
    virtual void preflight();


  signals:
    void updateFilterParameters(AbstractFilter* filter);
    void parametersChanged();
    void preflightAboutToExecute();
    void preflightExecuted();

  protected:
    WritePoleFigure();

    void dataCheck();

    /**
     * @brief generateImagePath Generates the path to write the image
     * @param label Name of file
     * @return Generated output path
     */
    QString generateImagePath(QString label);

    /**
     * @brief writeImage Writes the pole figure image to a file
     * @param image Image to write
     * @param label Name of file
     */
    void writeImage(QImage image, QString label);

  private:
    DEFINE_REQUIRED_DATAARRAY_VARIABLE(float, CellEulerAngles)
    DEFINE_REQUIRED_DATAARRAY_VARIABLE(int32_t, CellPhases)
    DEFINE_REQUIRED_DATAARRAY_VARIABLE(uint32_t, CrystalStructures)
    DEFINE_REQUIRED_DATAARRAY_VARIABLE(bool, GoodVoxels)

    WritePoleFigure(const WritePoleFigure&); // Copy Constructor Not Implemented
    void operator=(const WritePoleFigure&); // Operator '=' Not Implemented
};

#endif /* _WritePoleFigure_H_ */
