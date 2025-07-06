#include "EditorInterface.h"

#include "Game.h"

EditorInterface::EditorInterface() {}

EditorInterface::~EditorInterface() {}

void EditorInterface::initialize(GLFWwindow* window) {
	FINFO("Editor > Initializing Editor-Interface with imgui...");
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsClassic();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void EditorInterface::update() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void EditorInterface::render() {
	ImGui::Begin("Lights");
	for (int i = 0; i < this->controllablePointLights.size(); i++) {
		PointLight* pl = this->controllablePointLights[i];
		ImGui::Text("  Point Light #" + (i + 1));
		ImGui::Text("Transform ->");
		ImGui::DragFloat3("Light Position", (float*)&pl->position, 0.2f);
		ImGui::Text("Point Light ->");
		ImGui::DragFloat3("Light Color", (float*)&pl->color, 0.2f);
		ImGui::SliderFloat("Constant", &pl->constant, 0.0f, 100.0f);
		ImGui::SliderFloat("Linear", &pl->linear, 0.0f, 100.0f);
		ImGui::SliderFloat("Quadratic", &pl->quadratic, 0.0f, 100.0f);
		ImGui::Checkbox("Blinn Phong", &pl->blinn);
		ImGui::NewLine();
	}
	ImGui::End();

	ImGui::Begin("Engine & Rendering Options");
	ImGui::Checkbox("Cast Shadows", &graphicsOptions.castShadows);
	ImGui::Checkbox("Wireframe Rendering", &graphicsOptions.wireframeMode);
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EditorInterface::add_point_light(PointLight* pl) {
	this->controllablePointLights.push_back(pl);
}
