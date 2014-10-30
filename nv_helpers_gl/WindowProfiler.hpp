/*
 * Copyright 1993-2014 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 *
 */

#ifndef NV_PROJECTBASE_INCLUDED
#define NV_PROJECTBASE_INCLUDED

#include "profiler.hpp"
#include <main.h>

// enable the NV_PROFILE_SECTION(name) macro, or turn it to a noop
#ifndef NV_SUPPORT_PROFILE
#define NV_SUPPORT_PROFILE 1
#endif

#ifndef NV_DEBUG_FILTER
#define NV_DEBUG_FILTER     1
#endif


#if NV_SUPPORT_PROFILE
#define NV_PROFILE_SECTION(name)   nv_helpers_gl::Profiler::Section _tempTimer(m_profiler ,name)
#define NV_PROFILE_SPLIT()         m_profiler.accumulationSplit()
#else
#define NV_PROFILE_SECTION(name)
#define NV_PROFILE_SPLIT()
#endif

namespace nv_helpers_gl
{

  /*
    Project by default quits with ESC
    and allows toggling vsync with V
  */

  class WindowProfiler : public NVPWindow {
  public:

    class Window
    {
    public:
      Window() 
        : m_mouseButtonFlags(0)
        , m_wheel(0)
      {
        memset(m_keyPressed, 0, sizeof(m_keyPressed));
        memset(m_keyToggled, 0, sizeof(m_keyToggled));
      }

      int         m_viewsize[2];
      int         m_mouseCurrent[2];
      int         m_mouseButtonFlags;
      int         m_wheel;

      bool m_keyPressed[KEY_LAST+1];
      bool m_keyToggled[KEY_LAST+1];

      bool onPress(int key) {
        return m_keyPressed[key] && m_keyToggled[key];
      }
    };

    WindowProfiler(bool singleThreaded = true, bool doSwap = true) 
      : m_profilerPrint(true)
      , m_vsync(false)
      , m_singleThreaded(singleThreaded)
      , m_doSwap(doSwap)
    {
      m_debugFilter = GL_DEBUG_SEVERITY_HIGH + NV_DEBUG_FILTER;
    }

    virtual bool begin() { return false; }
    virtual void end() {}
    virtual void think(double time) {}
    virtual void resize(int width, int height) {}

    // return true to prevent m_window updates
    virtual bool mouse_pos    (int x, int y) {return false; }
    virtual bool mouse_button (int button, int action) {return false; }
    virtual bool mouse_wheel  (int wheel) {return false; }
    virtual bool key_button   (int button, int action, int modifier) {return false; }
    virtual bool key_char     (int button) {return false; }

    // from NVPWindow
    //void shutdown() {}
    void reshape(int w, int h);
    void motion(int x, int y);
    void mousewheel(int delta);
    void mouse(MouseButton button, ButtonAction action, int mods, int x, int y);
    void keyboard(KeyCode key, ButtonAction action, int mods, int x, int y);
    void keyboardchar(unsigned char key, int mods, int x, int y);
    //void display() { } // leave empty, we call redraw ourselves in think

    Window        m_window;
    Profiler      m_profiler;
    bool          m_profilerPrint;
    bool          m_singleThreaded;
    bool          m_doSwap;

    void vsync(bool state);
    void waitEvents();

    int run(const std::string &name,
      int argc, const char** argv, 
      int width, int height,
      int Major, int Minor);

  private:
    bool          m_vsync;
  };
}


#endif

