#ifndef PROX_GUI_WIDGET_H
#define PROX_GUI_WIDGET_H

#include <QTimer>
#include <QGLWidget>
#include <QApplication>
#include <QMouseEvent>

namespace prox_gui
{

  class Widget
  : public QGLWidget
  {
    Q_OBJECT

  private:

    //QOpenGLContext m__context;  // QWindow way
    
    QTimer        m_timer;

  public:

    Widget( QGLFormat const & format, QWidget *parent = 0);
    ~Widget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    public slots:

    void animate();

    //signals:

  protected:

    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *e);


  public:

    void save_screen(QString const & filename);

  };

}// namespace prox_gui

// PROX_GUI_WIDGET_H
#endif
