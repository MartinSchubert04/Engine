#pragma once

#include <vector>
#include "common.h"
#include <sstream>

namespace UI {

struct Console {

  std::vector<std::pair<int, std::string>> items;  // 0: Info, 1: Error

  void log(std::string msg, int type = 0) { items.push_back({type, msg}); }

  void draw() {
    ImGui::Begin("Console");
    if (ImGui::Button("Clear"))
      items.clear();
    ImGui::Separator();

    ImGui::BeginChild("scrolling", ImVec2(0, 0), false,
                      ImGuiWindowFlags_HorizontalScrollbar);
    for (auto &item : items) {
      if (item.first == 1)
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.4f, 0.4f, 1));

      // Al usar Selectable, puedes hacer click y copiar (Ctrl+C funcionará si
      // lo programas)
      if (ImGui::Selectable(item.second.c_str())) {
        ImGui::SetClipboardText(
            item.second.c_str());  // Copia al portapapeles al hacer clic
      }

      if (item.first == 1)
        ImGui::PopStyleColor();
    }

    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
      ImGui::SetScrollHereY(1.0f);

    ImGui::EndChild();
    ImGui::End();
  }
};

// Este objeto intercepta std::cout y lo manda a nuestra consola de ImGui
struct LogRedirector : public std::stringbuf {
  Console *console;
  std::ostream &stream;
  std::streambuf *old_buf;

  LogRedirector(std::ostream &src, UI::Console *target)
      : stream(src), console(target) {
    old_buf = stream.rdbuf(this);  // Redirigir
  }

  ~LogRedirector() {
    stream.rdbuf(old_buf);  // Restaurar al cerrar
  }

  // Se llama cada vez que std::cout recibe texto
  int sync() override {
    std::string msg = str();
    if (!msg.empty()) {
      console->log(msg, (stream.tie() == &std::cerr) ? 1 : 0);
      str("");  // Limpiar buffer interno
    }
    return 0;
  }
};

}  // namespace UI
