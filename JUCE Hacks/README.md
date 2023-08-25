This tweaked version of *juce_TextEditor.h* replaces the one in JUCE *modules/juce_gui_basics/widgets*. It has one change at line 1420; the original line

​    `auto caretRect = getCaretRectangle().translated (leftIndent, topIndent);`

is changed to

​    `auto caretRect = getCaretRectangle().translated (leftIndent, topIndent) - getTextOffset();`

in order to make the function *juce::TextEditor::scrollEditorToPositionCaret()* work as documented.

