#include <gl3.h>

#include <prox_gui_application.h>
#include <prox_gui_widget.h>

#include <util_log.h>

#include <cmath>

namespace prox_gui
{
  
  Widget::Widget( QGLFormat const & format, QWidget *parent)
  : QGLWidget(format, parent)
  , m_timer(this)
  {
    /**
     * QWindow way of doing GL initialization
     *
     * QSurfaceFormat format;
     * format.setVersion(3,3);
     * format.setProfile(QSurfaceFormat::CoreProfile);
     *
     * this->setSurfaceType(OpenGLSurface);
     * this->setFormat(format);
     * this->create();
     *
     * m_context->setFormat(format);
     * m_context->create();
     * m_context->makeCurrent(this);
     */
    
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(animate()));
    
    int const msecs =  1000.0/60.0;  // 60 FPS
    m_timer.start(msecs);
    m_timer.stop();  // 2010-10-25 Kenny: Make sure simulation is not running when app starts up
    
    Instance::app().init();
    Instance::app().gl_widget = this;
  }
  
  Widget::~Widget()
  {
  }
  
  QSize Widget::minimumSizeHint() const
  {
    return QSize(50, 50);
  }
  
  QSize Widget::sizeHint() const
  {
    return QSize(1024, 768);
  }
  
  void Widget::animate()
  {
    bool finished = ! Instance::app().run();
    update();
    if ( finished )
    {
      m_timer.stop();
    }
  }
  
  void Widget::resizeGL(int width, int height)
  {
    Instance::app().resize( width, height );
    
    glViewport(0, 0, width, height);
  }
  
  void Widget::initializeGL()
  {
    gl3::check_errors("initializeGL(): invoked");
    
    glewExperimental = GL_TRUE; // Needed in core profile
    int err = glewInit();
    
    gl3::check_errors("initializeGL(): glewInit");
    
    if ( GLEW_OK != err )
    {
      // problem: glewInit failed, something is seriously wrong
      util::Log logging;
      
      logging << "Widget::initializeGL(): GLEW Error: " << glewGetErrorString( err ) << util::Log::newline();
      
      exit( 1 );
    }
    gl3::check_errors("initializeGL(): GLEW Done 1");
    gl3::check_errors("initializeGL(): GLEW Done 2");
    
    GLubyte const * vendor   = glGetString (GL_VENDOR);
    gl3::check_errors("initializeGL(): glGetString 1");
    GLubyte const * renderer = glGetString (GL_RENDERER);
    gl3::check_errors("initializeGL(): glGetString 2");
    GLubyte const * version  = glGetString (GL_VERSION);
    gl3::check_errors("initializeGL(): glGetString 3");
    GLubyte const * glsl_ver = glGetString (GL_SHADING_LANGUAGE_VERSION);
    gl3::check_errors("initializeGL(): glGetString 4");
    
    {
      util::Log logging;
      
      logging << "Widget::initializeGL():" << util::Log::newline();
      logging << "  GLEW status: Using GLEW " << glewGetString( GLEW_VERSION ) << util::Log::newline();
      logging << "  Vendor       : " << vendor << util::Log::newline();
      logging << "  Renderer     : " << renderer << util::Log::newline();
      logging << "  GL version   : " << version << util::Log::newline();
      logging << "  GLSL version : " << glsl_ver << util::Log::newline();
    }
    
    glClearColor( 0.2, 0.2, 0.2, 1.0 );
    gl3::check_errors("initializeGL(): glColor");
    
    glEnable( GL_DEPTH_TEST );
    gl3::check_errors("initializeGL(): glEnable GL_DEPTH_TEST");
    
    glDepthFunc(GL_LESS);
    gl3::check_errors("initializeGL(): glDepthFunc");
    
    glEnable(GL_CULL_FACE);
    gl3::check_errors("initializeGL(): glEnable GL_CULL_FACE");
    
    glEnable(GL_BLEND);
    gl3::check_errors("initializeGL(): glEnable GL_BLEND");
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    gl3::check_errors("initializeGL(): glBlendFunc");
  }
  
  void Widget::paintGL()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    using std::min;
    using std::max;
    
    Instance::app().paint();
  }
  
  void Widget::mousePressEvent(QMouseEvent *event)
  {
    int    const ratio = devicePixelRatio();
    double const cur_x = event->x()*ratio;
    double const cur_y = event->y()*ratio;
    
    //  bool const meta  = QApplication::keyboardModifiers().testFlag(Qt::MetaModifier);
    bool const ctrl  = QApplication::keyboardModifiers().testFlag(Qt::ControlModifier);
    bool const alt   = QApplication::keyboardModifiers().testFlag(Qt::AltModifier);
    bool const shift = QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier);
    bool const left  = (event->buttons() & Qt::LeftButton);
    bool const right = (event->buttons() & Qt::RightButton);
    bool const midle = (event->buttons() & Qt::MiddleButton);
    
    Instance::app().mouse_down(cur_x,cur_y,shift,ctrl,alt,left,midle,right);
    update();
  }
  
  void Widget::mouseMoveEvent(QMouseEvent *event)
  {
    int    const ratio = devicePixelRatio();
    double const cur_x = event->x()*ratio;
    double const cur_y = event->y()*ratio;
    
    //  bool const meta  = QApplication::keyboardModifiers().testFlag(Qt::MetaModifier);
    //  bool const ctrl  = QApplication::keyboardModifiers().testFlag(Qt::ControlModifier);
    //  bool const alt   = QApplication::keyboardModifiers().testFlag(Qt::AltModifier);
    //  bool const shift = QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier);
    //  bool const left  = (event->buttons() & Qt::LeftButton);
    //  bool const right = (event->buttons() & Qt::RightButton);
    //  bool const midle = (event->buttons() & Qt::MiddleButton);
    
    Instance::app().mouse_move(cur_x,cur_y);
    update();
  }
  
  void Widget::mouseReleaseEvent(QMouseEvent *event)
  {
    int    const ratio = devicePixelRatio();
    double const cur_x = event->x()*ratio;
    double const cur_y = event->y()*ratio;
    
    //  bool const meta  = QApplication::keyboardModifiers().testFlag(Qt::MetaModifier);
    bool const ctrl  = QApplication::keyboardModifiers().testFlag(Qt::ControlModifier);
    bool const alt   = QApplication::keyboardModifiers().testFlag(Qt::AltModifier);
    bool const shift = QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier);
    bool const left  = (event->buttons() & Qt::LeftButton);
    bool const right = (event->buttons() & Qt::RightButton);
    bool const midle = (event->buttons() & Qt::MiddleButton);
    
    Instance::app().mouse_up(cur_x,cur_y,shift,ctrl,alt,left,midle,right);
    update();
  }
  
  void Widget::keyPressEvent(QKeyEvent *e)
  {
    if (! e->text().toStdString().empty() )
    {
      util::Log logging;
      
      logging << "Widget::keyPressEvent(): key = " << e->text().toStdString() << util::Log::newline();
    }
    
    unsigned char key = e->text().toStdString()[0];
    
    if (e->key() == Qt::Key_Space)
    {
      if(m_timer.isActive())
        m_timer.stop();
      else
        m_timer.start();
    }
    else if (e->key() == Qt::Key_Escape)
      close();
    else
      Instance::app().key_down( key );
    update();
  }
  
  
  void Widget::save_screen(QString const & filename)
  {
    this->grabFrameBuffer().save(filename);
  }
  
}// namespace prox_gui
