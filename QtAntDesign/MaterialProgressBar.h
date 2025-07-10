#ifndef MATERIALPROGRESSBAR_H
#define MATERIALPROGRESSBAR_H

#include <QProgressBar>
#include <QVariantAnimation>

class MaterialProgressBar : public QProgressBar
{
	Q_OBJECT
public:
	explicit MaterialProgressBar(QWidget* parent = nullptr);

	QSize sizeHint() const override;
	void setProgressAnimated(int targetValue);
	// ���ý���
	void setRealProgress(int targetValue);
	// ��ȷ������
	void startIndeterminate()
	{
		if (m_indeterminateAnimation->state() != QAbstractAnimation::Running)
		{
			m_isBeginIndeterminate = true;
			m_indeterminateAnimation->start();
		}
	}

	void stopIndeterminate()
	{
		if (m_indeterminateAnimation->state() == QAbstractAnimation::Running)
		{
			m_isBeginIndeterminate = false;
			m_indeterminateAnimation->stop();
			m_indeterminatePosition = 0.0;
			update();
		}
	}
	// ������ʾģʽ ��ʾ�������������� ʵ�ʿ�����Ҫ��
	void startTestPattern();
protected:
	void paintEvent(QPaintEvent* event) override;
private:
	void updateProgress();
private:
	QColor m_backgroundColor;  // ������ɫ
	QColor m_chunkColor;       // ������ɫ
	QVariantAnimation* m_animation = nullptr;

	// ��ȷ������
	bool m_isBeginIndeterminate = false;
	QVariantAnimation* m_indeterminateAnimation = nullptr;
	qreal m_indeterminatePosition = 0;  // 0.0 ~ 1.0 ��������ȱ���
	// ��ʾ��ֵ
	int currentValue = 0;
};

#endif // MATERIALPROGRESSBAR_H
