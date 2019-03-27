#pragma once
#include"Scene.h"
#include<glfw3.h>



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

MyScene scene;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main(void)
{
	//创建窗口
	GLFWwindow* window;

	//初始化glfw
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
	if (!window)
	{
		//销毁所有剩下的窗口和游标，恢复所有被修改的伽马线，并释放所有其他分配的资源
		glfwTerminate();
		return -1;
	}

	/* 该函数指定在当前调用线程上的窗口渲染环境为OpenGL或OpenGL ES环境 */
	glfwMakeContextCurrent(window);

	//指定事件
	glfwSetKeyCallback(window, key_callback);			//键盘事件
														//glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);



														//创建场景

	scene.Init();






	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		//计算一帧的间隔
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;



		//场景更新和绘制
		scene.Update();
		scene.Draw();

		//交换双缓冲
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
		//glfwWaitEvents();
	}

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	float cameraSpeed = 20.f*deltaTime;		//设置一个相机移动速度，一帧的时间作为单位
	if (key == GLFW_KEY_W&&action == GLFW_REPEAT)
		scene.camera.Walk(cameraSpeed);
	if (key == GLFW_KEY_S&&action == GLFW_REPEAT)
		scene.camera.Walk(-cameraSpeed);
	if (key == GLFW_KEY_A&&action == GLFW_REPEAT)
		scene.camera.LRMove(cameraSpeed);
	if (key == GLFW_KEY_D&&action == GLFW_REPEAT)
		scene.camera.LRMove(-cameraSpeed);
	if (key == GLFW_KEY_Q&&action == GLFW_REPEAT)
		scene.camera.LRRotate(cameraSpeed);
	if (key == GLFW_KEY_E&&action == GLFW_REPEAT)
		scene.camera.LRRotate(-cameraSpeed);
}
