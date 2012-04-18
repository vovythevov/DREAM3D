/* ============================================================================
 * Copyright (c) 2011, Michael A. Jackson (BlueQuartz Software)
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

#ifndef QLoadSlices_H_
#define QLoadSlices_H_

#include <QtCore/QObject>
#include <QtGui/QFileDialog>

#include "FilterWidgets/QFilterWidget.h"
#include "DREAM3DLib/Common/DREAM3DSetGetMacros.h"
#include "DREAM3DLib/ReconstructionFilters/LoadSlices.h"

#include "FilterWidgets/ui_QLoadSlicesWidget.h"

class QualityMetricTableModel;


class QLoadSlicesWidget : public QFilterWidget, private Ui::QLoadSlicesWidget
{
  Q_OBJECT;
  public:
    QLoadSlicesWidget(QWidget* parent = NULL);
    virtual ~QLoadSlicesWidget();

    virtual AbstractFilter::Pointer getFilter();


//    Q_PROPERTY(QString H5EbsdFile READ getH5EbsdFile WRITE setH5EbsdFile)
//    void setH5EbsdFile(QString v) { m_H5EbsdFile = v; }
//    QString  getH5EbsdFile() { return m_H5EbsdFile; }

//    Q_PROPERTY(int ZStartIndex READ getZStartIndex WRITE setZStartIndex)
//    DREAM3D_INSTANCE_PROPERTY(int, ZStartIndex) ;
//
//    Q_PROPERTY(int ZEndIndex READ getZEndIndex WRITE setZEndIndex)
//    DREAM3D_INSTANCE_PROPERTY(int, ZEndIndex) ;

    virtual void writeOptions(QSettings &prefs);
    virtual void readOptions(QSettings &prefs);

  protected slots:

    void on_addQualityMetric_clicked();

    void on_removeQualityMetric_clicked();

    void m_SetSliceInfo();

    // Auto Hookup Slots
    void on_m_H5EbsdBtn_clicked();
    void on_m_H5EbsdFile_textChanged(const QString &text);


  private slots:
    void phaseTypeEdited(int i);

  private:
    QualityMetricTableModel*    m_QualityMetricTableModel;
    bool                        m_phaseTypeEdited;
    bool                        rotateslice;
    bool                        reorderarray;
    bool                        aligneulers;
    QString                     m_OpenDialogLastDirectory;
 //   QString                     m_H5EbsdFile;

    void setupGui();
    bool verifyPathExists(QString outFilePath, QLineEdit* lineEdit);

    /**
     * @brief
     * @return
     */
    bool checkPhaseTypes();

    /**
     * @brief
     * @param r
     */
    void setupQualityMetricFilters(LoadSlices::Pointer filter);

    QLoadSlicesWidget(const QLoadSlicesWidget&); // Copy Constructor Not Implemented
    void operator=(const QLoadSlicesWidget&); // Operator '=' Not Implemented
};


#endif /* QLoadSlices_H_ */
