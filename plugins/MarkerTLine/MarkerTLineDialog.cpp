/*
 *  QtTrader stock charter
 *
 *  Copyright (C) 2001-2010 Stefan S. Stratigakos
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
 *  USA.
 */

#include "MarkerTLineDialog.h"

#include "Global.h"

#include <QtDebug>
#include <QSettings>
#include <QLayout>
#include <QFormLayout>


MarkerTLineDialog::MarkerTLineDialog (QWidget *p) : QDialog (p)
{
  _settings = 0;
  _modified = 0;
  connect(this, SIGNAL(finished(int)), this, SLOT(deleteLater()));

  // main vbox
  QVBoxLayout *vbox = new QVBoxLayout;
  vbox->setSpacing(2);
  vbox->setMargin(5);
  setLayout(vbox);
  
  _tabs = new QTabWidget;
  vbox->addWidget(_tabs);

  // buttonbox
  _buttonBox = new QDialogButtonBox(QDialogButtonBox::Help);
  connect(_buttonBox, SIGNAL(accepted()), this, SLOT(done()));
  connect(_buttonBox, SIGNAL(rejected()), this, SLOT(cancel()));
  vbox->addWidget(_buttonBox);

  // ok button
  _okButton = _buttonBox->addButton(QDialogButtonBox::Ok);
  _okButton->setText(tr("&OK"));

  // cancel button
  _cancelButton = _buttonBox->addButton(QDialogButtonBox::Cancel);
  _cancelButton->setText(tr("&Cancel"));
  _cancelButton->setDefault(true);
  _cancelButton->setFocus();
}

void
MarkerTLineDialog::done ()
{
  saveSettings();
  save();
  accept();
}

void
MarkerTLineDialog::cancel ()
{
  saveSettings();
  reject();
}

void
MarkerTLineDialog::loadSettings ()
{
  QSettings settings(g_settings);
  settings.beginGroup(g_session);

  QSize sz = settings.value("marker_tline_dialog_size", QSize(200,200)).toSize();
  resize(sz);

  // restore the position of the app
  QPoint p = settings.value("marker_tline_dialog_pos").toPoint();
  if (! p.isNull())
    move(p);
}

void
MarkerTLineDialog::saveSettings ()
{
  QSettings settings(g_settings);
  settings.beginGroup(g_session);
  settings.setValue("marker_tline_dialog_size", size());
  settings.setValue("marker_tline_dialog_pos", pos());
}

void
MarkerTLineDialog::setGUI (Entity *settings)
{
  _settings = settings;

  // OHLC page
  QWidget *w = new QWidget;
  
  QFormLayout *form = new QFormLayout;
  w->setLayout(form);
  
  // color
  QVariant *set = _settings->get(QString("color"));
  if (! set)
  {
    qDebug() << "MarkerTLineDialog::setGUI: no color setting";
    return;
  }
  
  _color = new ColorButton(this, QColor(set->toString()));
  connect(_color, SIGNAL(valueChanged(QColor)), this, SLOT(valueChanged()));
  form->addRow(tr("Color"), _color);
  
  // date
  set = _settings->get(QString("date"));
  if (! set)
  {
    qDebug() << "MarkerTLineDialog::setGUI: no date setting";
    return;
  }
  
  _date = new QDateTimeEdit;
  _date->setDateTime(set->toDateTime());
  connect(_date, SIGNAL(dateTimeChanged(const QDateTime &)), this, SLOT(valueChanged()));
  form->addRow(tr("Start Date"), _date);
  
  // date2
  set = _settings->get(QString("date2"));
  if (! set)
  {
    qDebug() << "MarkerTLineDialog::setGUI: no date2 setting";
    return;
  }
  
  _date2 = new QDateTimeEdit;
  _date2->setDateTime(set->toDateTime());
  connect(_date2, SIGNAL(dateTimeChanged(const QDateTime &)), this, SLOT(valueChanged()));
  form->addRow(tr("End Date"), _date2);
  
  // price
  set = _settings->get(QString("price"));
  if (! set)
  {
    qDebug() << "MarkerTLineDialog::setGUI: no price setting";
    return;
  }

  _price = new QDoubleSpinBox;
  _price->setRange(-99999999.0, 99999999.0);
  _price->setValue(set->toDouble());
  connect(_price, SIGNAL(valueChanged(const QString &)), this, SLOT(valueChanged()));
  form->addRow(tr("Start Price"), _price);
  
  // price2
  set = _settings->get(QString("price2"));
  if (! set)
  {
    qDebug() << "MarkerTLineDialog::setGUI: no price2 setting";
    return;
  }

  _price2 = new QDoubleSpinBox;
  _price2->setRange(-99999999.0, 99999999.0);
  _price2->setValue(set->toDouble());
  connect(_price2, SIGNAL(valueChanged(const QString &)), this, SLOT(valueChanged()));
  form->addRow(tr("End Price"), _price2);

  _tabs->addTab(w, tr("Settings"));
}

void MarkerTLineDialog::save ()
{
  if (! _modified)
    return;

  QVariant *set = _settings->get(QString("color"));
  set->setValue(_color->color().name());

  set = _settings->get(QString("date"));
  set->setValue(_date->dateTime());

  set = _settings->get(QString("date2"));
  set->setValue(_date2->dateTime());

  set = _settings->get(QString("price"));
  set->setValue(_price->value());

  set = _settings->get(QString("price2"));
  set->setValue(_price2->value());
}

void MarkerTLineDialog::valueChanged ()
{
  _modified = 1;
}
