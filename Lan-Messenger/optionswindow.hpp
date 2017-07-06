#ifndef OPTIONSWINDOW_HPP
#define OPTIONSWINDOW_HPP

#include <QWidget>

namespace Ui {
	class OptionsWindow;
}

class OptionsWindow : public QWidget
{
		Q_OBJECT

	public:
		explicit OptionsWindow(QWidget *parent = 0);
		~OptionsWindow();

	private:
		Ui::OptionsWindow *ui;
};

#endif // OPTIONSWINDOW_HPP
