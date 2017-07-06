#include "optionswindow.hpp"
#include "ui_optionswindow.h"

OptionsWindow::OptionsWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::OptionsWindow)
{
	ui->setupUi(this);
	ui->leID->setText("");
}

OptionsWindow::~OptionsWindow()
{
	delete ui;
	connect(this->ui->leID, SIGNAL(returnPressed()), this, SLOT(onNameChanged(QString)));
}

void OptionsWindow::onNameChanged(QString name)
{
	emit nameSet(name);
}
