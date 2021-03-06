#pragma once

#include "ega.h"

#define UI_DRAGDROP_PALCOLOR "EGAPALCOLOR"
#define UI_DRAGDROP_SNIPPET "EGASNIPPET"

struct uiDragDropPalColor {
   EGAColor color;         // 0-63
   EGAPColor paletteIndex; // use EGA_PALETTE_COLORS for null
};

typedef struct Window Window;
typedef struct GameData GameData;
void uiBimpStart(Window* wnd); 
void uiBimpHandleDrop(Window* wnd);
void uiBimpStartEX(Window* wnd, EGATexture const *texture, EGAPalette *palette, Float2 cursorPos);

enum PaletteEditorFlags_ {
   PaletteEditorFlags_ENCODE = (1 << 0), // adds encode sentinel values to color picker
   PaletteEditorFlags_POPPED_OUT = (1 << 1)// Dont use, set by popout
};
typedef byte PaletteEditorFlags;

void uiPaletteEditor(Window* wnd, EGAPalette* pal, char* palName = nullptr, u32 palNameSize = 0, PaletteEditorFlags flags = 0);
void uiPaletteColorPicker(StringView label, EGAColor *color);
float uiPaletteEditorWidth();
float uiPaletteEditorHeight();

enum uiModalResults_ {
   uiModalResults_OK = 0,  // user clicked OK
   uiModalResults_CANCEL,  //user clicked Cancel
   uiModalResults_YES,     //"    "       Yes
   uiModalResults_NO,      //"    "       No

   uiModalResults_OPEN,    // Status, user clicked nothing, dlg still open
   uiModalResults_CLOSED   // Status, popup isnt open!
};
typedef byte uiModalResult;

enum uiModalTypes_ {
   uiModalTypes_YESNO,
   uiModalTypes_YESNOCANCEL,
   uiModalTypes_OK,
   uiModalTypes_OKCANCEL
};
typedef byte uiModalType;

// Opens an ImGui modal popup with some basic emssagebox features
// THIS REQUIRES THAT YOU HAVE CALLED ImGui::OpenPopup() with the same title or this does nothing
// remember title is both the window title and the ID, use ## to differentiate, imgui shit etc
// keyboard shortcuts:
// for OK and OKCANCEL, spacebar or enter returns OK
// for YESNOCANCEL and CANCEL, ESC returns cancel
// for YESNO, ESC returns no
uiModalResult uiModalPopup(StringView title, StringView msg, uiModalType type = uiModalTypes_OK, StringView icon = nullptr);
