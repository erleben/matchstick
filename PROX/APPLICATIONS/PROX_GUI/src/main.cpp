#include <prox_gui_widget.h>

#include <QApplication>
#include <QDesktopWidget>

#include <QtOpenGL/QGLFormat>

#include <cstring>

int main(int argc, char *argv[])
{
  int fullscreen = -1;
  for (int i = 0; i < argc && fullscreen != 0; ++i)
  {
    fullscreen = strcmp(argv[i], "--fullscreen");
  }
    
  QApplication app(argc, argv);

  QGLFormat format;
  format.setVersion( 4, 1 );
  format.setProfile( QGLFormat::CoreProfile ); // Requires >=Qt-4.8.0
  format.setDoubleBuffer(true);
  format.setDepth(true);
  format.setDepthBufferSize(24);
  format.setOption(QGL::NoDeprecatedFunctions );

  // Create a GLWidget requesting our format
  prox_gui::Widget window(format);
  
  if (fullscreen == 0)
  {
    window.showFullScreen();
  }
  else
  {
    window.resize(window.sizeHint());

    int desktopArea = QApplication::desktop()->width() * QApplication::desktop()->height();

    int widgetArea = window.width() * window.height();

    if (((float)widgetArea / (float)desktopArea) < 0.75f)
      window.show();
    else
      window.showMaximized();
  }

  return app.exec();
}
