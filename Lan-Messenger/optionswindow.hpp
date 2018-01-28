#ifndef OPTIONSWINDOW_HPP
#define OPTIONSWINDOW_HPP

#include <QWidget>
#include <QString>

namespace Ui {
	class OptionsWindow;
}

class OptionsWindow : public QWidget
{
		Q_OBJECT

	public:
		explicit OptionsWindow(QWidget *parent = nullptr);
		~OptionsWindow();

	signals:
		void nameSet(QString name);

	private slots:
		void onNameChanged(QString);

	private:

		Ui::OptionsWindow *ui;
};

#endif // OPTIONSWINDOW_HPP
