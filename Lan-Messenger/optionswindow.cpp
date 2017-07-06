#include "optionswindow.hpp"
#include "ui_optionswindow.h"

OptionsWindow::OptionsWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::OptionsWindow)
{
	ui->setupUi(this);
}

OptionsWindow::~OptionsWindow()
{
	delete ui;
}
