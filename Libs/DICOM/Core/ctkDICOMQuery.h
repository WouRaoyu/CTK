/*=========================================================================

  Library:   CTK

  Copyright (c) Kitware Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0.txt

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

=========================================================================*/

#ifndef __ctkDICOMQuery_h
#define __ctkDICOMQuery_h

// Qt includes 
#include <QObject>
#include <QMap>
#include <QString>
#include <QSqlDatabase>

// CTK includes
#include <ctkPimpl.h>

#include "ctkDICOMCoreExport.h"
#include "ctkDICOMDatabase.h"

class ctkDICOMQueryPrivate;

/// \ingroup DICOM_Core
class CTK_DICOM_CORE_EXPORT ctkDICOMQuery : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString callingAETitle READ callingAETitle WRITE setCallingAETitle);
  Q_PROPERTY(QString calledAETitle READ calledAETitle WRITE setCalledAETitle);
  Q_PROPERTY(QString host READ host WRITE setHost);
  Q_PROPERTY(int port READ port WRITE setPort);
  Q_PROPERTY(bool preferCGET READ preferCGET WRITE setPreferCGET);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
  Q_PROPERTY(QList< QPair<QString,QString> > studyAndSeriesInstanceUIDQueried READ studyAndSeriesInstanceUIDQueried);
  Q_PROPERTY(QMap<QString, QVariant> filters READ filters WRITE setFilters);
#else
  Q_PROPERTY(QList studyAndSeriesInstanceUIDQueried READ studyAndSeriesInstanceUIDQueried);
  Q_PROPERTY(QMap filters READ filters WRITE setFilters);
#endif

public:
  explicit ctkDICOMQuery(QObject* parent = 0);
  virtual ~ctkDICOMQuery();
  
  /// Set methods for connectivity
  /// Empty by default
  void setCallingAETitle ( const QString& callingAETitle );
  QString callingAETitle()const;
  /// Empty by default
  void setCalledAETitle ( const QString& calledAETitle );
  QString calledAETitle()const;
  /// Empty by default
  void setHost ( const QString& host );
  QString host()const;
  /// Specify a port for the packet headers.
  /// \a port ranges from 0 to 65535.
  /// 0 by default.
  void setPort ( int port );
  int port()const;
  /// Prefer CGET over CMOVE for retrieval of query results
  /// false by default
  void setPreferCGET ( bool preferCGET );
  bool preferCGET()const;

  /// Query a remote DICOM Image Store SCP
  /// You must at least set the host and port before calling query()
  Q_INVOKABLE bool query(ctkDICOMDatabase& database);

  /// Access the list of study and series instance UIDs from the last query
  QList< QPair<QString,QString> >  studyAndSeriesInstanceUIDQueried()const;

  ///
  /// Filters are keyword/value pairs as generated by
  /// the ctkDICOMWidgets in a human readable (and editable)
  /// format.  The Query is responsible for converting these
  /// into the appropriate dicom syntax for the C-Find
  /// Currently supports the keys: Name, Study, Series, ID, Modalities,
  /// StartDate and EndDate
  /// Key         DICOM Tag                Type        Example
  /// -----------------------------------------------------------
  /// Name        DCM_PatientName         QString     JOHNDOE
  /// Study       DCM_StudyDescription     QString
  /// Series      DCM_SeriesDescription    QString
  /// ID          DCM_PatientID            QString
  /// Modalities  DCM_ModalitiesInStudy    QStringList CT, MR, MN
  /// StartDate   DCM_StudyDate            QString     20090101
  /// EndDate     DCM_StudyDate            QString     20091231
  /// No filter (empty) by default.
  void setFilters(const QMap<QString,QVariant>&);
  QMap<QString,QVariant> filters()const;

Q_SIGNALS:
  /// Signal is emitted inside the query() function. It ranges from 0 to 100.
  /// In case of an error, you are assured that the progress value 100 is fired
  void progress(int progress);
  /// Signal is emitted inside the query() function. It sends the different step
  /// the function is at.
  void progress(const QString& message);
  /// Signal is emitted inside the query() function. It sends 
  /// detailed feedback for debugging
  void debug(const QString& message);
  /// Signal is emitted inside the query() function. It send any error messages
  void error(const QString& message);
  /// Signal is emitted inside the query() function when finished with value 
  /// true for success or false for error
  void done(const bool& error);

public Q_SLOTS:
  void cancel();

protected:
  QScopedPointer<ctkDICOMQueryPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(ctkDICOMQuery);
  Q_DISABLE_COPY(ctkDICOMQuery);

  friend class ctkDICOMQuerySCUPrivate;  // for access to queryResponseHandled
};

#endif
