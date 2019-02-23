#pragma once

#include "PCH.h"
#include "Event.h"

namespace Bat
{
	class Input : public EventDispatcher
	{
		friend class Window;
	public:
		Input();
		Input( const Input& src ) = delete;
		Input& operator=( const Input& src ) = delete;
		Input( Input&& donor ) = delete;
		Input& operator=( Input&& donor ) = delete;
	private: // keyboard		
		void OnKeyChar( const size_t key, bool repeated );
		void OnKeyDown( const size_t key, bool repeated );
		void OnKeyUp( const size_t key );
	public:
		bool IsKeyDown( const size_t key ) const;
		bool IsAutorepeatEnabled() const { return m_bEnableAutorepeat; };
		void SetAutorepeatEnabled( bool enabled ) { m_bEnableAutorepeat = enabled; }
	public: // mouse
		enum class MouseButton
		{
			Left,
			Right,
			Middle,
			X1,
			X2,
			TOTAL_MOUSE_BUTTONS
		};
	private:
		void OnMouseMoved( const Vei2& pos );
		void OnMouseWheelScrolled( const Vei2& pos, const float delta );
		void OnMouseButtonDown( const Vei2& pos, const MouseButton mb );
		void OnMouseButtonUp( const Vei2& pos, const MouseButton mb );
		void OnMouseButtonDblClick( const Vei2& pos, const MouseButton mb );
		void OnMouseEnter();
		void OnMouseLeave();
	public:
		Vei2 GetMousePosition() const;
		bool IsMouseButtonDown( const MouseButton mb ) const;

		bool IsLeftDown() const;
		bool IsRightDown() const;
		bool IsMiddleDown() const;
		bool IsX1Down() const;
		bool IsX2Down() const;

		bool IsMouseInWindow() const;
	private:
		static constexpr int MaxKeys = 256;
		bool m_bKeyIsPressed[MaxKeys];
		bool m_bEnableAutorepeat = false;

		Vei2 m_vecMousePosition;
		bool m_bMouseButtonIsDown[(int)MouseButton::TOTAL_MOUSE_BUTTONS];
		bool m_bMouseInWindow = false;
	};
}