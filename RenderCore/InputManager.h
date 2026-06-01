#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

namespace rcore {

  class InputManager {
  private:
    static InputManager* instance;

  public:
    static InputManager& get();

  private:
    InputManager();

  public:
    void keyDown(unsigned int input);
    void keyUp(unsigned int input);
    bool isKeyDown(unsigned int key) const;

  private:
    bool m_keys[256];
  };

}

#endif;