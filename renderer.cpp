#include "renderer.h"



// constructor
Renderer::Renderer(QWidget *parent)
    : QOpenGLWidget(parent)
{

    grabKeyboard();

    activeCamera = new Camera(vec3(0,0, -1), vec3(0.31649,-0.564746,4.26627));
    bsplineGeometry = new BSplineGenerator();

//    /w = parent->width();
    //h = parent->height();
}

Renderer::~Renderer()
{
    delete activeCamera;
    delete bsplineGeometry;
}

void Renderer::setBig_R_value(double R)
{
    this->R = R;
    //update();
}

void Renderer::setSmall_r_value(double r)
{
    this->r = r;
    //update();
}

void Renderer::setN_value(double n)
{
    this->n =n;
}

void Renderer::redrawGraph()
{
    graphLines.clear();

    graphLines.push_back(vec3(0,0,0));
    graphLines.push_back(vec3(0,0,1));
    update();
    std::cout << "The graph should redraw.. \n";
    //glUseProgram(programID);
    //glBindBuffer(GL_ARRAY_BUFFER, graphVbo);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * graphLines.size(), graphLines.data(), GL_STATIC_DRAW);



}

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());

    //QOpenGLContext *m_context = new QOpenGLContext();
    //m_context->makeCurrent(this);
    // Create the shaders
    GLuint VertexShaderID = glFuncs.glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glFuncs.glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if(VertexShaderStream.is_open()){
        std::string Line = "";
        while(getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }else{
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
        getchar();
        return 0;
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::string Line = "";
        while(getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;


    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glFuncs.glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glFuncs.glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glFuncs.glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glFuncs.glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        glFuncs.glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }



    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glFuncs.glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glFuncs.glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glFuncs.glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glFuncs.glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glFuncs.glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }



    // Link the program
    printf("Linking program\n");
    GLuint ProgramID = glFuncs.glCreateProgram();
    glFuncs.glAttachShader(ProgramID, VertexShaderID);
    glFuncs.glAttachShader(ProgramID, FragmentShaderID);
    glFuncs.glLinkProgram(ProgramID);

    // Check the program
    glFuncs.glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glFuncs.glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        glFuncs.glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }


    glFuncs.glDetachShader(ProgramID, VertexShaderID);
    glFuncs.glDetachShader(ProgramID, FragmentShaderID);

    glFuncs.glDeleteShader(VertexShaderID);
    glFuncs.glDeleteShader(FragmentShaderID);

    return ProgramID;
}




void Renderer::generateGraph(float theta, int n, float angleMultiplier)
{
    while (theta < n * PI * 2 )
    {
        glColor3f(1.0, 0.0, 0.0);

        float x = (R - r) * cos(theta) + r * cos(angleMultiplier * theta);
        float y = (R - r) * sin(theta) - r * sin(angleMultiplier * theta);
        //glVertex3d(x, y, 0.0);
        graphLines.push_back(glm::vec3(x, y, 0));
        theta += 0.1;
    }
}

void Renderer :: initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0,0,0,1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);




    programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );


    r = 1.0;
    R = 3.0;

    float theta;
    int n;

    theta = 0.0;


    n = 200.0;

    float angleMultiplier = (R - r)/r;

    //graphLines.push_back(glm::vec3(0,0, 0));
    //graphLines.push_back(glm::vec3(0,-1,0));

    //graphLines.push_back(glm::vec3(0,0,0));
    //graphLines.push_back(glm::vec3(0,1,0));
    //graphLines.push_back(vec3(1,1,0));
    //graphLines.push_back(glm::vec3(1,1,0));
    //graphLines.push_back(glm::vec3(0,0,1));



    generateGraph(theta, n, angleMultiplier);


    glGenVertexArrays(1, &graphVao);
    glBindVertexArray(graphVao);

    glGenBuffers(1, &graphVbo);
    glBindBuffer(GL_ARRAY_BUFFER, graphVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * graphLines.size(),
                 graphLines.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);


    glGenVertexArrays(1, &pointsVao);
    glBindVertexArray(pointsVao);

    glGenBuffers(1, &pointsVbo);
    glBindBuffer(GL_ARRAY_BUFFER, pointsVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * bsplineGeometry->getNumberOfControlPoints(),
                 bsplineGeometry->getControlPoints().data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);


    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(33); // 33 ms
}

void Renderer :: paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(graphVao);
    // Use the normal shader
    glUseProgram(programID);

    //glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, graphVbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * graphLines.size(),
                 graphLines.data(), GL_STATIC_DRAW);

    glm::mat4x4 perpectiveMatrix = winRatio * perspective * activeCamera->getMatrix();


    glUniformMatrix4fv(glGetUniformLocation(programID, "modelviewMatrix"), 1,
                    false, &modelview[0][0]);

    glUniformMatrix4fv(glGetUniformLocation(programID, "perspectiveMatrix" ), 1,
                       false,
                       &perpectiveMatrix[0][0]);



    glDrawArrays(GL_LINE_STRIP, 0, graphLines.size());

    glBindVertexArray(0);

    glBindVertexArray(pointsVao);

    glUseProgram(programID);

    glBindBuffer(GL_ARRAY_BUFFER, pointsVbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * bsplineGeometry->getControlPoints().size(),
                 bsplineGeometry->getControlPoints().data(),GL_STATIC_DRAW);



    glUniformMatrix4fv(glGetUniformLocation(programID, "modelviewMatrix"), 1,
                    false, &modelview[0][0]);

    glUniformMatrix4fv(glGetUniformLocation(programID, "perspectiveMatrix" ), 1,
                       false,
                       &perpectiveMatrix[0][0]);

    glDrawArrays(GL_POINTS, 0, bsplineGeometry->getNumberOfControlPoints());
    glBindVertexArray(0);




    //glBindVertexArray(0);

        // Swap buffers
   //glfwSwapBuffers();
        //glfwPollEvents();











}

void Renderer:: resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w/h, 0.01, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,0,0,0,0,0,1,0);

    //paintGL();
}

void Renderer::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_A)
    {

        if (animation)
        {
            animation = false;
            t = graphLines.size();
        } else {
            t = 0;
            animation = true;
        }
        //t = graphLines.size();
    }
}

void Renderer::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_R)
    {
        //std::cout << "R key has been pressed \n";
        //redraw the graph geometry
        bool done;

        std::cout << "Enter in the r value: \n";
        float r_value;
        std::cin >> r_value;
        r = (double) r_value;

        std::cout << "Enter in the R value: ";
        float R_value;
        std::cin >> R_value;
        R = (double) R_value;

        std::cout << "Enter in the n value: ";
        std::cin >> n;
        graphLines.clear();
        generateGraph(0, n, (R - r)/r);
        update();
    } else if (event->key() == Qt::Key_Q)
       {
        activeCamera->trackballRight(0.1);
       } else if (event->key() == Qt::Key_W)
    {
        activeCamera->trackballUp(0.1);
    }
}

// override mouse press event
void Renderer::mousePressEvent(QMouseEvent * event)
{

    bsplineGeometry->selected = -1;

    double x = (2 * ((double) event->x())/((double) this->window()->width() ) -1);
    double y = (-2 * ((double) event->y())/((double) this->window()->height()) + 1);

    vector<vec3> controlPoints = bsplineGeometry->getControlPoints();
    for (int i = 0; i < controlPoints.size(); i++)
    {
        if (abs(controlPoints.at(i).x - x) <= cRadius
                && abs(controlPoints.at(i).y - y) <= cRadius)
        {
            bsplineGeometry->selected = i;
            std::cout <<  bsplineGeometry->selected << endl;
        }
    }

    if (event->button() == Qt::LeftButton)
    {
        if (bsplineGeometry->selected = -1)
            bsplineGeometry->addControlPoint(vec3(x * this->window()->width(),
                                                  y * this->window()->height(),0));
    }

    if (event->button() == Qt::RightButton)
    {
        if (bsplineGeometry->selected != -1)
        {
            //remove the control point
        }
    }

    if (event->button() == Qt::RightButton)
    {
     rightMousePressed =true;
    } else if (event->button()== Qt::LeftButton)
    {
        leftMousePressed = true;
    }
}

void Renderer::mouseMoveEvent(QMouseEvent *event)
{
    int vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);

    glm::vec2 newPos = vec2(event->x()/(double)vp[2], -event->y()/(double)vp[3]) * 2.f- vec2(1.f);

    glm::vec2 diff = newPos -mousePos;

    if (leftMousePressed){
        activeCamera->trackballRight(-diff.x);
        activeCamera->trackballUp(-diff.y);
    } else{
        float zoomBase =(diff.y > 0) ? 1.f/2.f :2.f;

        activeCamera->zoom(pow(zoomBase, abs(diff.y)));
    }

    mousePos = newPos;

    update();
}

void Renderer::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        rightMousePressed =false;
    } else if (event->button()== Qt::LeftButton)
    {
        leftMousePressed = false;
    }

    //int vp[4];
    //mousePos = vec2(event->x()/(double)vp[2], event->y()/(double) vp[3]) * 2.f - vec2(1.f);
}
