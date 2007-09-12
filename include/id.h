/* Liste des identificateurs des boutons et menus */
#ifndef ID_H
#define ID_H

#define MAX_ITEMS_IN_PICKER     15  ///< max no. items in the popup menu for item selection

enum main_id {
	ID_MAIN_FRAME = 100,
	ID_LEFT_FRAME,
	ID_BOTTOM_FRAME,
	ID_MAIN_DIALOG,
	ID_PROJECT_TREE,
	ID_PROJECT_TXTEDIT,
	ID_PROJECT_TREE_REFRESH,
	ID_PROJECT_RUNPY,
	ID_PROJECT_NEWFILE,
	ID_PROJECT_UNUSED0,
	ID_PROJECT_UNUSED1,
	ID_PROJECT_NEWPY,
	ID_PROJECT_UNUSED2,
	ID_PROJECT_NEWTXT,
	ID_PROJECT_UNUSED3,
	ID_PROJECT_NEWDIR,
	ID_PROJECT_DELETE,
	ID_PROJECT_RENAME,
	ID_PROJECT_OPEN_FILE_WITH_TEXT_EDITOR,
	ID_PROJECT_UNUSED4,

	ID_MAIN_COMMAND,
	ID_TO_EDITOR,
	ID_TO_EESCHEMA,
	ID_TO_GERBVIEW,
	ID_TO_PCB,
	ID_TO_CVPCB,
	ID_RUN_PYTHON,
	ID_MAIN_MENUBAR,
	ID_ON_ZOOM_SELECT,
	ID_ON_GRID_SELECT,
	ID_BROWSE_AN_SELECT_FILE,
	ID_SELECT_PREFERED_EDITOR,
	ID_SELECT_PREFERED_PDF_BROWSER_NAME,
	ID_SELECT_PREFERED_PDF_BROWSER,
	ID_SELECT_DEFAULT_PDF_BROWSER,
	ID_SAVE_AND_ZIP_FILES,
	ID_READ_ZIP_ARCHIVE,

	// ID du menu general
	ID_LOAD_PROJECT,
	ID_LOAD_ONE_SHEET,
	ID_NEW_PROJECT,
	ID_LOAD_FILE_1,
	ID_LOAD_FILE_2,
	ID_LOAD_FILE_3,
	ID_LOAD_FILE_4,
	ID_LOAD_FILE_5,
	ID_LOAD_FILE_6,
	ID_LOAD_FILE_7,
	ID_LOAD_FILE_8,
	ID_LOAD_FILE_9,
	ID_LOAD_FILE_10,

	ID_SAVE_PROJECT,
	ID_SAVE_ONE_SHEET,
	ID_SAVE_ONE_SHEET_AS,

	ID_CONFIG_REQ,
	ID_CONFIG_SAVE,
	ID_CONFIG_READ,
	ID_PREFERENCES_CREATE_CONFIG_HOTKEYS,
	ID_PREFERENCES_READ_CONFIG_HOTKEYS,
	ID_PREFERENCES_UNUSED0,
	ID_PREFERENCES_UNUSED1,
	ID_PREFERENCES_UNUSED2,
	ID_PREFERENCES_UNUSED3,

	ID_GEN_PRINT,
	ID_GEN_PLOT,
	ID_GEN_PLOT_PS,
	ID_GEN_PLOT_HPGL,
	ID_GEN_PLOT_GERBER,
	ID_GEN_PLOT_SVG,
	ID_GEN_COPY_SHEET_TO_CLIPBOARD,
	ID_GEN_COPY_BLOCK_TO_CLIPBOARD,
	ID_GEN_UNUSED0,
	ID_GEN_UNUSED1,
	ID_GEN_UNUSED2,
	ID_GEN_UNUSED3,
	ID_GEN_UNUSED4,

	ID_GEN_EXPORT_FILE,
	ID_GEN_EXPORT_FILE_GENCADFORMAT,
	ID_GEN_EXPORT_FILE_MODULE_REPORT,
	ID_GEN_EXPORT_FILE_FORMAT_2,		// Not used
	ID_GEN_EXPORT_FILE_FORMAT_3,		// Not used
	ID_GEN_EXPORT_FILE_FORMAT_4,		// Not used

	ID_EXIT,
	ID_OPTIONS_SETUP,

	ID_PREFERENCES_FONT,
	ID_PREFERENCES_FONT_INFOSCREEN,
	ID_PREFERENCES_FONT_DIALOG,
	ID_PREFERENCES_FONT_STATUS,
	ID_PREFERENCES_FONT_UNUSED3,
	ID_PREFERENCES_FONT_UNUSED4,
	ID_PREFERENCES_FONT_UNUSED5,
	ID_PREFERENCES_FONT_UNUSED6,
	ID_PREFERENCES_FONT_UNUSED7,
	ID_PREFERENCES_FONT_END,

	// Find id menu
	ID_H_TOOLBAR,
	ID_V_TOOLBAR,
	ID_OPT_TOOLBAR,
	ID_AUX_TOOLBAR,
	ID_AUX_V_TOOLBAR,
	ID_TOOLBAR_UNUSED,

	ID_GENERAL_HELP,
	ID_LOCAL_HELP,
	ID_KICAD_ABOUT,

	ID_EDIT,

	ID_SEL_BG_COLOR,

	ID_COLORS_SETUP,

	ID_REPEAT_BUTT,
	ID_UNDO_BUTT,
	ID_REDO_BUTT,

	ID_LANGUAGE_CHOICE,
	ID_LANGUAGE_DEFAULT,
	ID_LANGUAGE_ENGLISH,
	ID_LANGUAGE_FRENCH,
	ID_LANGUAGE_SPANISH,
	ID_LANGUAGE_GERMAN,
	ID_LANGUAGE_RUSSIAN,
	ID_LANGUAGE_PORTUGUESE,
	ID_LANGUAGE_ITALIAN,
	ID_LANGUAGE_SLOVENIAN,
	ID_LANGUAGE_HUNGARIAN,
	ID_LANGUAGE_POLISH,
	ID_LANGUAGE_KOREAN,
	ID_LANGUAGE_UNUSED2,
	ID_LANGUAGE_UNUSED3,
	ID_LANGUAGE_UNUSED4,
	ID_LANGUAGE_CHOICE_END,


	// ID du Main Toolbar de Schematique
	
	ID_SCHEMATIC_MAIN_TOOLBAR_START,
	ID_SCHEMATIC_UNDO,
	ID_SCHEMATIC_REDO,
	ID_SCHEMATIC_MAIN_TOOLBAR_END,

	// ID du Vertical Toolbar de Schematique
	ID_SCHEMATIC_VERTICAL_TOOLBAR_START,
	ID_NO_SELECT_BUTT,
	ID_HIERARCHY_PUSH_POP_BUTT,
	ID_COMPONENT_BUTT,
	ID_PLACE_POWER_BUTT,
	ID_BUS_BUTT,
	ID_WIRE_BUTT,
	ID_BUSTOBUS_ENTRY_BUTT,
	ID_WIRETOBUS_ENTRY_BUTT,
	ID_LABEL_BUTT,
	ID_GLABEL_BUTT,
	ID_IMPORT_GLABEL_BUTT,
	ID_SHEET_LABEL_BUTT,
	ID_NOCONN_BUTT,
	ID_JUNCTION_BUTT,
	ID_SHEET_SYMBOL_BUTT,
	ID_TEXT_COMMENT_BUTT,
	ID_LINE_COMMENT_BUTT,
	ID_SCHEMATIC_DELETE_ITEM_BUTT,
	ID_SCHEMATIC_UNUSED0,
	ID_SCHEMATIC_UNUSED1,
	ID_SCHEMATIC_UNUSED2,
	ID_SCHEMATIC_VERTICAL_TOOLBAR_END,
	// end ID Vertical Toolbar de Schematique

	ID_SET_REPEAT_OPTION,

	// Popup Menu (mouse Right button) (id consecutifs)

	ID_POPUP_GENERAL_START_RANGE,	// first number
	ID_POPUP_CANCEL_CURRENT_COMMAND,   // ID general annulation commande en cours
	ID_POPUP_CLOSE_CURRENT_TOOL,
	ID_POPUP_MOVE_BLOCK,
	ID_POPUP_DRAG_BLOCK,
	ID_POPUP_COPY_BLOCK,
	ID_POPUP_ROTATE_BLOCK,
	ID_POPUP_DELETE_BLOCK,
	ID_POPUP_INVERT_BLOCK,
	ID_POPUP_PLACE_BLOCK,
	ID_POPUP_ZOOM_BLOCK,
	ID_POPUP_SELECT_ITEMS_BLOCK,
	ID_POPUP_MIRROR_X_BLOCK,
	ID_POPUP_MIRROR_Y_BLOCK,
	ID_POPUP_MIRROR_UNUSED0,
	ID_POPUP_MIRROR_UNUSED1,
	ID_POPUP_MIRROR_UNUSED2,
	ID_POPUP_MIRROR_UNUSED3,
	ID_POPUP_OTHER_COMMANDS,
	ID_POPUP_GENERAL_END_RANGE,	// last number

	ID_POPUP_ENTER_MENU,

	ID_POPUP_ZOOM_START_RANGE,	// first number
	ID_POPUP_CANCEL,
	ID_POPUP_ZOOM_PLUS,
	ID_POPUP_ZOOM_MOINS,
	ID_POPUP_ZOOM_SELECT,
	ID_POPUP_ZOOM_CENTER,
	ID_POPUP_ZOOM_AUTO,
	ID_POPUP_ZOOM_REDRAW,
	ID_POPUP_ZOOM_LEVEL_1,
	ID_POPUP_ZOOM_LEVEL_2,
	ID_POPUP_ZOOM_LEVEL_4,
	ID_POPUP_ZOOM_LEVEL_8,
	ID_POPUP_ZOOM_LEVEL_16,
	ID_POPUP_ZOOM_LEVEL_32,
	ID_POPUP_ZOOM_LEVEL_64,
	ID_POPUP_ZOOM_LEVEL_128,
	ID_POPUP_ZOOM_LEVEL_256,
	ID_POPUP_ZOOM_LEVEL_512,
	ID_POPUP_ZOOM_LEVEL_1024,
	ID_POPUP_ZOOM_LEVEL_2048,
	ID_POPUP_GRID_PLUS,
	ID_POPUP_GRID_MOINS,
	ID_POPUP_GRID_SELECT,
	ID_POPUP_GRID_LEVEL_1000,
	ID_POPUP_GRID_LEVEL_500,
	ID_POPUP_GRID_LEVEL_250,
	ID_POPUP_GRID_LEVEL_200,
	ID_POPUP_GRID_LEVEL_100,
	ID_POPUP_GRID_LEVEL_50,
	ID_POPUP_GRID_LEVEL_25,
	ID_POPUP_GRID_LEVEL_20,
	ID_POPUP_GRID_LEVEL_10,
	ID_POPUP_GRID_LEVEL_5,
	ID_POPUP_GRID_LEVEL_2,
	ID_POPUP_GRID_LEVEL_1,
	ID_POPUP_GRID_USER,
	ID_POPUP_ZOOM_UNUSED0,
	ID_POPUP_ZOOM_UNUSED1,
	ID_POPUP_ZOOM_END_RANGE,				// last number


	ID_POPUP_START_RANGE,					// first number
	ID_POPUP_LIBEDIT_PIN_EDIT,				// Id pour menu PopUp de Libedit
	ID_POPUP_LIBEDIT_PIN_GLOBAL_CHANGE_ITEM,
	ID_POPUP_LIBEDIT_PIN_GLOBAL_CHANGE_PINSIZE_ITEM,
	ID_POPUP_LIBEDIT_PIN_GLOBAL_CHANGE_PINNAMESIZE_ITEM,
	ID_POPUP_LIBEDIT_PIN_GLOBAL_CHANGE_PINNUMSIZE_ITEM,
	ID_POPUP_LIBEDIT_BODY_EDIT_ITEM,		// Id pour menu PopUp de Libedit
	ID_POPUP_LIBEDIT_DELETE_ITEM,
	ID_POPUP_LIBEDIT_END_CREATE_ITEM,
	ID_POPUP_LIBEDIT_CANCEL_EDITING,
	ID_POPUP_LIBEDIT_MOVE_ITEM_REQUEST,
	ID_POPUP_LIBEDIT_FIELD_EDIT_ITEM,
	ID_POPUP_LIBEDIT_FIELD_ROTATE_ITEM,

	ID_POPUP_LIBEDIT_DELETE_CURRENT_POLY_SEGMENT,
	ID_POPUP_LIBEDIT_ROTATE_GRAPHIC_TEXT,
	ID_POPUP_LIBEDIT_UNUSED2,				// En attente d'affectation
	ID_POPUP_LIBEDIT_UNUSED3,				// En attente d'affectation
	ID_POPUP_LIBEDIT_UNUSED4,				// En attente d'affectation
	ID_POPUP_LIBEDIT_UNUSED5,				// En attente d'affectation

	ID_POPUP_SCH_DELETE,
	ID_POPUP_SCH_BREAK_WIRE,
	ID_POPUP_SCH_DELETE_CONNECTION,
	ID_POPUP_SCH_MOVE_ITEM_REQUEST,
	ID_POPUP_SCH_DELETE_NODE,
	ID_POPUP_SCH_MOVE_CMP_REQUEST,
	ID_POPUP_SCH_DELETE_CMP,
	ID_POPUP_SCH_UNUSED_0,
	ID_POPUP_SCH_UNUSED_1,
	ID_POPUP_SCH_UNUSED_2,
	ID_POPUP_SCH_ENTRY_SELECT_SLASH,
	ID_POPUP_SCH_ENTRY_SELECT_ANTISLASH,

	ID_POPUP_SCH_EDIT_CMP,
	ID_POPUP_SCH_MIROR_X_CMP,
	ID_POPUP_SCH_MIROR_Y_CMP,
	ID_POPUP_SCH_ROTATE_CMP_CLOCKWISE,
	ID_POPUP_SCH_ROTATE_CMP_COUNTERCLOCKWISE,
	ID_POPUP_SCH_ORIENT_NORMAL_CMP,
	ID_POPUP_SCH_INIT_CMP,

	ID_POPUP_SCH_EDIT_TEXT,
	ID_POPUP_SCH_ROTATE_TEXT,
	ID_POPUP_SCH_SET_SHAPE_TEXT,
	ID_POPUP_END_LINE,
	ID_POPUP_SCH_END_SHEET,
	ID_POPUP_SCH_EDIT_SHEET,
	ID_POPUP_SCH_RESIZE_SHEET,
	ID_POPUP_SCH_CLEANUP_SHEET,
	ID_POPUP_SCH_EDIT_PINSHEET,
	ID_POPUP_SCH_MOVE_PINSHEET,
	ID_POPUP_SCH_GENERIC_ORIENT_CMP,
	ID_POPUP_SCH_GENERIC_EDIT_CMP,
	ID_POPUP_SCH_EDIT_VALUE_CMP,
	ID_POPUP_SCH_EDIT_REF_CMP,
	ID_POPUP_SCH_EDIT_CONVERT_CMP,
	ID_POPUP_SCH_COPY_COMPONENT_CMP,
	ID_POPUP_SCH_SELECT_UNIT_CMP,
	ID_POPUP_SCH_SELECT_UNIT1,
	ID_POPUP_SCH_SELECT_UNIT2,
	ID_POPUP_SCH_SELECT_UNIT3,
	ID_POPUP_SCH_SELECT_UNIT4,
	ID_POPUP_SCH_SELECT_UNIT5,
	ID_POPUP_SCH_SELECT_UNIT6,
	ID_POPUP_SCH_SELECT_UNIT7,
	ID_POPUP_SCH_SELECT_UNIT8,
	ID_POPUP_SCH_SELECT_UNIT9,
	ID_POPUP_SCH_SELECT_UNIT10,
	ID_POPUP_SCH_SELECT_UNIT11,
	ID_POPUP_SCH_SELECT_UNIT12,
	ID_POPUP_SCH_SELECT_UNIT13,
	ID_POPUP_SCH_SELECT_UNIT14,
	ID_POPUP_SCH_SELECT_UNIT15,
	ID_POPUP_SCH_SELECT_UNIT16,
	ID_POPUP_SCH_SELECT_UNIT17,
	ID_POPUP_SCH_SELECT_UNIT18,
	ID_POPUP_SCH_SELECT_UNIT19,
	ID_POPUP_SCH_SELECT_UNIT20,
	ID_POPUP_SCH_SELECT_UNIT21,
	ID_POPUP_SCH_SELECT_UNIT22,
	ID_POPUP_SCH_SELECT_UNIT23,
	ID_POPUP_SCH_SELECT_UNIT24,
	ID_POPUP_SCH_SELECT_UNIT25,
	ID_POPUP_SCH_SELECT_UNIT26,
	ID_POPUP_SCH_ROTATE_FIELD,
	ID_POPUP_SCH_EDIT_FIELD,
	ID_POPUP_SCH_CHANGE_TYPE_TEXT,
	ID_POPUP_SCH_CHANGE_TYPE_TEXT_TO_LABEL,
	ID_POPUP_SCH_CHANGE_TYPE_TEXT_TO_GLABEL,
	ID_POPUP_SCH_CHANGE_TYPE_TEXT_TO_COMMENT,
	ID_POPUP_SCH_DISPLAYDOC_CMP,
	ID_POPUP_SCH_ENTER_SHEET,
	ID_POPUP_SCH_LEAVE_SHEET,
	ID_POPUP_SCH_ADD_JUNCTION,
	ID_POPUP_SCH_ADD_LABEL,
	ID_POPUP_SCH_ADD_GLABEL,
	ID_POPUP_SCH_UNUSED0,
	ID_POPUP_SCH_UNUSED1,
	ID_POPUP_SCH_UNUSED2,
	ID_POPUP_SCH_UNUSED3,

	ID_POPUP_END_RANGE,				// last number

	// Horizontal Toolbar de schematique
	ID_SHEET_SET,
	ID_HIERARCHY,
	ID_TO_LIBRARY,
	ID_TO_LIBVIEW,

	ID_ZOOM_PLUS_BUTT,	// Gestion zoom: id consecutifs
	ID_ZOOM_MOINS_BUTT,
	ID_ZOOM_REDRAW_BUTT,
	ID_ZOOM_PAGE_BUTT,
	ID_ZOOM_WINDOW_BUTT,
	ID_ZOOM_PLUS_KEY,
	ID_ZOOM_MOINS_KEY,
	ID_ZOOM_REDRAW_KEY,
	ID_ZOOM_CENTER_KEY,
	ID_ZOOM_AUTO,
	ID_ZOOM_PANNING_UP,
	ID_ZOOM_PANNING_DOWN,
	ID_ZOOM_PANNING_LEFT,
	ID_ZOOM_PANNING_RIGHT,
	ID_ZOOM_UNUSED0,
	ID_ZOOM_UNUSED1,
	ID_ZOOM_UNUSED2,
	ID_ZOOM_UNUSED3,
	ID_ZOOM_ENDLIST,

	ID_GET_ANNOTATE,	// Gestion fonctions : id consecutifs
	ID_GET_ERC,
	ID_GET_NETLIST,
	ID_GET_TOOLS,
	ID_FIND_ITEMS,

	// Id pour HToolBar de Libview (Process_Special_Functions)
	ID_LIBVIEW_START_H_TOOL,
	ID_LIBVIEW_NEXT,
	ID_LIBVIEW_PREVIOUS,
	ID_LIBVIEW_SELECT_PART,
	ID_LIBVIEW_SELECT_LIB,
	ID_LIBVIEW_VIEWDOC,
	ID_LIBVIEW_DE_MORGAN_NORMAL_BUTT,
	ID_LIBVIEW_DE_MORGAN_CONVERT_BUTT,
	ID_LIBVIEW_TOOL_UNUSED0,
	ID_LIBVIEW_TOOL_UNUSED1,
	ID_LIBVIEW_END_H_TOOL,
	ID_LIBVIEW_SELECT_PART_NUMBER,
	ID_LIBVIEW_LIB_LIST,
	ID_LIBVIEW_CMP_LIST,
	ID_LIBVIEW_UNUSED0,
	ID_LIBVIEW_UNUSED1,
	ID_LIBVIEW_UNUSED3,
	ID_LIBVIEW_UNUSED4,
	ID_LIBVIEW_CMP_EXPORT_TO_SCHEMATIC,	// Used in Htoolbar, specific function

	/* LibEdit: */
	/* Id for HToolBar Tools( buttons) for Libedit (Process_Special_Functions) */
	ID_LIBEDIT_START_H_TOOL,
	ID_LIBEDIT_SELECT_PART,
	ID_LIBEDIT_SELECT_CURRENT_LIB,
	ID_LIBEDIT_SAVE_CURRENT_LIB,
	ID_LIBEDIT_SAVE_CURRENT_PART,
	ID_LIBEDIT_NEW_PART,
	ID_LIBEDIT_GET_FRAME_EDIT_PART,
	ID_LIBEDIT_DELETE_PART,
	ID_LIBEDIT_IMPORT_PART,
	ID_LIBEDIT_EXPORT_PART,
	ID_LIBEDIT_CREATE_NEW_LIB_AND_SAVE_CURRENT_PART,
	ID_LIBEDIT_UNDO,
	ID_LIBEDIT_REDO,
	ID_DE_MORGAN_NORMAL_BUTT,
	ID_DE_MORGAN_CONVERT_BUTT,
	ID_LIBEDIT_EDIT_PIN_BY_PIN,
	ID_LIBEDIT_VIEW_DOC,
    ID_LIBEDIT_CHECK_PART,
	ID_LIBEDIT_END_H_TOOL,	// End Id for HToolBar (Libedit)
	ID_LIBEDIT_SELECT_PART_NUMBER, // Id selection unit of part (HToolBar, combo box)
	ID_LIBEDIT_SELECT_ALIAS, // Id selection alias of part (HToolBar, combo box)
	ID_LIBEDIT_HTOOL_UNUSED1,
	ID_LIBEDIT_HTOOL_UNUSED2,
    ID_LIBEDIT_HTOOL_UNUSED3,
    ID_LIBEDIT_HTOOL_UNUSED4,

	/* Id pour VToolBar de Libedit */
	ID_LIBEDIT_START_V_TOOL,
	ID_LIBEDIT_PIN_BUTT,
	ID_LIBEDIT_BODY_LINE_BUTT,
	ID_LIBEDIT_BODY_ARC_BUTT,
	ID_LIBEDIT_BODY_CIRCLE_BUTT,
	ID_LIBEDIT_BODY_RECT_BUTT,
	ID_LIBEDIT_BODY_TEXT_BUTT,
	ID_LIBEDIT_DELETE_ITEM_BUTT,
	ID_LIBEDIT_ANCHOR_ITEM_BUTT,
	ID_LIBEDIT_IMPORT_BODY_BUTT,
	ID_LIBEDIT_EXPORT_BODY_BUTT,
	ID_LIBEDIT_VTOOL_UNUSED0,
	ID_LIBEDIT_VTOOL_UNUSED1,
	ID_LIBEDIT_VTOOL_UNUSED2,
	ID_LIBEDIT_VTOOL_UNUSED3,
	ID_LIBEDIT_VTOOL_UNUSED4,
	ID_LIBEDIT_END_V_TOOL, // End Id pour VToolBar de Libedit

	// ID pour CVPCB
	ID_CVPCB_START_TOOL,
	ID_CVPCB_QUIT,
	ID_CVPCB_READ_INPUT_NETLIST,
	ID_CVPCB_SAVEQUITCVPCB,
	ID_CVPCB_CREATE_CONFIGWINDOW,
	ID_CVPCB_CREATE_SCREENCMP,
	ID_CVPCB_GOTO_FIRSTNA,
	ID_CVPCB_GOTO_PREVIOUSNA,
	ID_CVPCB_DEL_ASSOCIATIONS,
	ID_CVPCB_AUTO_ASSOCIE,
	ID_CVPCB_DISPLAY_HELP,
	ID_CVPCB_DISPLAY_LICENCE,
	ID_CVPCB_END_TOOL,
	ID_CVPCB_COMPONENT_LIST,
	ID_CVPCB_FOOTPRINT_LIST,
	ID_CVPCB_CREATE_STUFF_FILE,
	ID_CVPCB_SHOW3D_FRAME,
	ID_CVPCB_FOOTPRINT_DISPLAY_FULL_LIST,
	ID_CVPCB_FOOTPRINT_DISPLAY_FILTERED_LIST,
	ID_CVPCB_UNUSED0,
	ID_CVPCB_UNUSED1,
	ID_CVPCB_UNUSED2,
	ID_CVPCB_UNUSED3,

	// id specifiques pcbnew
	ID_LOAD_FILE,
	ID_APPEND_FILE,
	ID_NEW_BOARD,
	ID_OPEN_MODULE_EDITOR,
	ID_UNNUSED_BUTT,
	ID_TRACK_BUTT,
	ID_PCB_ZONES_BUTT,
	ID_PCB_DELETE_ITEM_BUTT,
	ID_READ_NETLIST,
	ID_DRC_CONTROL,
	ID_PCB_CIRCLE_BUTT,
	ID_PCB_ARC_BUTT,
	ID_PCB_HIGHLIGHT_BUTT,
	ID_PCB_MIRE_BUTT,
	ID_PCB_SHOW_1_RATSNEST_BUTT,
	ID_PCB_PLACE_OFFSET_COORD_BUTT,
	ID_PCB_UNUSED_BUTT0,
	ID_PCB_UNUSED_BUTT1,
	ID_PCB_UNUSED_BUTT2,
	ID_PCB_UNUSED_BUTT3,

	ID_POPUP_PCB_START_RANGE,
	ID_POPUP_PCB_MOVE_MODULE_REQUEST,
	ID_POPUP_PCB_DRAG_MODULE_REQUEST,
	ID_POPUP_PCB_EDIT_MODULE,
	ID_POPUP_PCB_CHANGE_SIDE_MODULE,
	ID_POPUP_PCB_DELETE_MODULE,
	ID_POPUP_PCB_ROTATE_MODULE_CLOCKWISE,
	ID_POPUP_PCB_ROTATE_MODULE_COUNTERCLOCKWISE,


	ID_POPUP_PCB_EDIT_PAD,
	ID_POPUP_PCB_EDIT_MORE_PAD,
	ID_POPUP_PCB_EDIT_PADS_ON_MODULE,
	ID_POPUP_PCB_DELETE_PAD,
	ID_POPUP_PCB_ADD_PAD,
	ID_POPUP_PCB_NEWSIZE_PAD,
	ID_POPUP_PCB_ROTATE_PAD,
	ID_POPUP_PCB_MOVE_PAD_REQUEST,
	ID_POPUP_PCB_DRAG_PAD_REQUEST,

	ID_POPUP_PCB_MOVE_TEXTMODULE_REQUEST,
	ID_POPUP_PCB_ROTATE_TEXTMODULE,
	ID_POPUP_PCB_EDIT_TEXTMODULE,
	ID_POPUP_PCB_DELETE_TEXTMODULE,

	ID_POPUP_PCB_MOVE_TEXTEPCB_REQUEST,
	ID_POPUP_PCB_ROTATE_TEXTEPCB,
	ID_POPUP_PCB_EDIT_TEXTEPCB,
	ID_POPUP_PCB_DELETE_TEXTEPCB,

	ID_POPUP_PCB_MOVE_DRAWING_REQUEST,
	ID_POPUP_PCB_EDIT_DRAWING,
	ID_POPUP_PCB_DELETE_DRAWING,
	ID_POPUP_PCB_DELETE_DRAWING_LAYER,

	ID_POPUP_PCB_EDIT_TRACK,
	ID_POPUP_PCB_DELETE_TRACKSEG,
	ID_POPUP_PCB_DELETE_TRACK,
	ID_POPUP_PCB_DELETE_TRACKNET,
	ID_POPUP_PCB_DELETE_TRACK_MNU,
	ID_POPUP_PCB_EDIT_ZONE,
	ID_POPUP_PCB_DELETE_ZONE,

	ID_POPUP_PCB_DELETE_MARKER,
	ID_POPUP_PCB_DELETE_COTATION,

	ID_POPUP_PCB_MOVE_MIRE_REQUEST,
	ID_POPUP_PCB_DELETE_MIRE,
	ID_POPUP_PCB_EDIT_MIRE,

	ID_POPUP_PCB_STOP_CURRENT_DRAWING,

	ID_POPUP_PCB_EDIT_COTATION,
	ID_POPUP_PCB_END_TRACK,
	ID_POPUP_PCB_PLACE_VIA,

	ID_POPUP_PCB_IMPORT_PAD_SETTINGS,
	ID_POPUP_PCB_EXPORT_PAD_SETTINGS,

	ID_POPUP_PCB_STOP_CURRENT_EDGE_ZONE,
	ID_POPUP_PCB_DELETE_EDGE_ZONE,
	ID_POPUP_PCB_DELETE_ZONE_LIMIT,
	ID_POPUP_PCB_FILL_ZONE,
	ID_POPUP_PCB_SELECT_NET_ZONE,

	ID_POPUP_PCB_SELECT_WIDTH,
	ID_POPUP_PCB_SELECT_WIDTH1,
	ID_POPUP_PCB_SELECT_WIDTH2,
	ID_POPUP_PCB_SELECT_WIDTH3,
	ID_POPUP_PCB_SELECT_WIDTH4,
	ID_POPUP_PCB_SELECT_WIDTH5,
	ID_POPUP_PCB_SELECT_WIDTH6,
	ID_POPUP_PCB_SELECT_WIDTH7,
	ID_POPUP_PCB_SELECT_WIDTH8,
	ID_POPUP_PCB_SELECT_VIASIZE,
	ID_POPUP_PCB_SELECT_VIASIZE1,
	ID_POPUP_PCB_SELECT_VIASIZE2,
	ID_POPUP_PCB_SELECT_VIASIZE3,
	ID_POPUP_PCB_SELECT_VIASIZE4,
	ID_POPUP_PCB_SELECT_VIASIZE5,
	ID_POPUP_PCB_SELECT_VIASIZE6,
	ID_POPUP_PCB_SELECT_VIASIZE7,
	ID_POPUP_PCB_SELECT_VIASIZE8,
	ID_POPUP_PCB_EDIT_TRACKSEG,
	ID_POPUP_PCB_EDIT_TRACK_MNU,
	ID_POPUP_PCB_EDIT_NET,
	ID_POPUP_PCB_SELECT_LAYER,
	ID_POPUP_PCB_SELECT_CU_LAYER,
	ID_POPUP_PCB_SELECT_NO_CU_LAYER,
	ID_POPUP_PCB_SELECT_LAYER_PAIR,
	ID_POPUP_PCB_EDIT_EDGE,
	ID_POPUP_PCB_DELETE_EDGE,
	ID_POPUP_PCB_MOVE_EDGE,
	ID_POPUP_PCB_PLACE_EDGE,
	ID_POPUP_PCB_END_EDGE,
	ID_POPUP_PCB_LOCK_ON_TRACKSEG,
	ID_POPUP_PCB_LOCK_OFF_TRACKSEG,
	ID_POPUP_PCB_LOCK_ON_TRACK,
	ID_POPUP_PCB_LOCK_OFF_TRACK,
	ID_POPUP_PCB_LOCK_ON_NET,
	ID_POPUP_PCB_LOCK_OFF_NET,
	ID_POPUP_PCB_SETFLAGS_TRACK_MNU,

	ID_POPUP_PCB_AUTOROUTE_GET_AUTOROUTER,
	ID_POPUP_PCB_AUTOROUTE_GET_AUTOROUTER_DATA,
	ID_POPUP_PCB_EDIT_WIDTH_CURRENT_EDGE,
	ID_POPUP_PCB_EDIT_WIDTH_ALL_EDGE,
	ID_POPUP_PCB_EDIT_LAYER_CURRENT_EDGE,
	ID_POPUP_PCB_EDIT_LAYER_ALL_EDGE,
	ID_POPUP_PCB_ENTER_EDGE_WIDTH,
	ID_POPUP_PCB_GLOBAL_IMPORT_PAD_SETTINGS,
	ID_POPUP_PCB_EDIT_ALL_VIAS_AND_TRACK_SIZE,
	ID_POPUP_PCB_EDIT_ALL_VIAS_SIZE,
	ID_POPUP_PCB_EDIT_ALL_TRACK_SIZE,
	ID_POPUP_PCB_DISPLAY_FOOTPRINT_DOC,
	ID_POPUP_PCB_MOVE_TRACK_NODE,
	ID_POPUP_PCB_BREAK_TRACK,
	ID_POPUP_PCB_PLACE_MOVED_TRACK_NODE,
	ID_POPUP_PCB_VIA_EDITING,
	ID_POPUP_PCB_VIA_HOLE_TO_DEFAULT,
	ID_POPUP_PCB_VIA_HOLE_TO_VALUE,
	ID_POPUP_PCB_VIA_HOLE_ENTER_VALUE,
	ID_POPUP_PCB_VIA_HOLE_EXPORT,
	ID_POPUP_PCB_VIA_HOLE_RESET_TO_DEFAULT,
	ID_POPUP_PCB_VIA_HOLE_EXPORT_TO_OTHERS,
	ID_POPUP_PCB_DRAG_TRACK_SEGMENT_KEEP_SLOPE,
	ID_POPUP_PCB_DRAG_TRACK_SEGMENT,
	ID_POPUP_PCB_MOVE_TRACK_SEGMENT,
	ID_POPUP_PCB_GET_AND_MOVE_MODULE_REQUEST,
	ID_POPUP_PCB_UNUSED5,
	ID_POPUP_PCB_UNUSED6,
	ID_POPUP_PCB_UNUSED7,
	ID_POPUP_PCB_UNUSED8,

	ID_POPUP_PCB_END_RANGE,

    // reserve a block of MAX_ITEMS_IN_PICKER ids for the item selection popup  
    ID_POPUP_PCB_ITEM_SELECTION_START,
    ID_POPUP_PCB_ITEM_SELECTION_END = MAX_ITEMS_IN_PICKER + ID_POPUP_PCB_ITEM_SELECTION_START,

	ID_POPUP_PCB_AUTOPLACE_START_RANGE,

	ID_POPUP_PCB_AUTOPLACE_FIXE_MODULE,
	ID_POPUP_PCB_AUTOPLACE_FREE_MODULE,
	ID_POPUP_PCB_AUTOPLACE_FREE_ALL_MODULES,
	ID_POPUP_PCB_AUTOPLACE_FIXE_ALL_MODULES,
	ID_POPUP_PCB_AUTOPLACE_CURRENT_MODULE,
	ID_POPUP_PCB_AUTOMOVE_ALL_MODULES,
	ID_POPUP_PCB_AUTOMOVE_NEW_MODULES,
	ID_POPUP_PCB_AUTOPLACE_COMMANDS,

	ID_POPUP_PCB_REORIENT_ALL_MODULES,

	ID_POPUP_PCB_AUTOPLACE_ALL_MODULES,
	ID_POPUP_PCB_AUTOPLACE_NEW_MODULES,
	ID_POPUP_PCB_AUTOPLACE_NEXT_MODULE,

	ID_POPUP_PCB_AUTOPLACE_UNUSED0,
	ID_POPUP_PCB_AUTOPLACE_UNUSED1,
	ID_POPUP_PCB_AUTOPLACE_UNUSED2,
	ID_POPUP_PCB_AUTOPLACE_UNUSED3,

	ID_POPUP_PCB_AUTOROUTE_COMMANDS,
	ID_POPUP_PCB_AUTOROUTE_ALL_MODULES,
	ID_POPUP_PCB_AUTOROUTE_MODULE,
	ID_POPUP_PCB_AUTOROUTE_PAD,
	ID_POPUP_PCB_AUTOROUTE_NET,

	ID_POPUP_PCB_AUTOROUTE_RESET_UNROUTED,

	ID_POPUP_PCB_AUTOROUTE_SELECT_LAYERS,
	ID_POPUP_PCB_AUTOROUTE_UNUSED1,
	ID_POPUP_PCB_AUTOROUTE_UNUSED2,
	ID_POPUP_PCB_AUTOROUTE_UNUSED3,
	ID_POPUP_PCB_AUTOROUTE_UNUSED4,
	ID_POPUP_PCB_AUTOROUTE_UNUSED5,
	ID_POPUP_PCB_AUTOROUTE_UNUSED6,
	ID_POPUP_PCB_AUTOROUTE_UNUSED7,

	ID_POPUP_PCB_AUTOPLACE_END_RANGE,

	ID_SAVE_BOARD,
	ID_MENU_SAVE_BOARD,
	ID_MENU_SAVE_BOARD_AS,
	ID_MENU_LOAD_FILE,
	ID_MENU_APPEND_FILE,
	ID_MENU_NEW_BOARD,
	ID_MENU_RECOVER_BOARD,
	ID_MENU_READ_LAST_SAVED_VERSION_BOARD,
	ID_MENU_ARCHIVE_MODULES,
	ID_MENU_ARCHIVE_NEW_MODULES,
	ID_MENU_ARCHIVE_ALL_MODULES,
	ID_MENU_MICELLANOUS,
	ID_MENU_LIST_NETS,
	ID_MENU_PCB_CLEAN,
	ID_MENU_PCB_SWAP_LAYERS,
	ID_MENU_PCB_UNUSED0,
	ID_MENU_PCB_UNUSED1,
	ID_MENU_PCB_UNUSED2,
	ID_MENU_PCB_UNUSED3,
	ID_MENU_PCB_UNUSED4,

	ID_TOOLBARH_PCB_SELECT_LAYER,
	ID_TOOLBARH_PCB_AUTOPLACE,
	ID_TOOLBARH_PCB_AUTOROUTE,
	ID_TOOLBARH_PCB_UNUSED,

	ID_AUX_TOOLBAR_PCB_VIA_SIZE,
	ID_AUX_TOOLBAR_PCB_TRACK_WIDTH,
	ID_AUX_TOOLBAR_PCB_SELECT_LAYER_PAIR,
	ID_AUX_TOOLBAR_PCB_UNUSED2,
	ID_AUX_TOOLBAR_PCB_UNUSED3,
	ID_AUX_TOOLBAR_PCB_UNUSED4,
	ID_AUX_TOOLBAR_PCB_UNUSED5,
	ID_AUX_TOOLBAR_PCB_UNUSED6,
	ID_AUX_TOOLBAR_PCB_UNUSED7,

	ID_PCB_GEN_POS_MODULES_FILE,
	ID_PCB_GEN_DRILL_FILE,

	ID_PCB_LOOK_SETUP,
	ID_PCB_TRACK_SIZE_SETUP,
	ID_PCB_PAD_SETUP,

	ID_PCB_GLOBAL_DELETE,
	ID_PCB_COTATION_BUTT,
	ID_PCB_DRAWINGS_WIDTHS_SETUP,

	ID_PCB_GEN_CMP_FILE,
	ID_MENU_PCB_SHOW_3D_FRAME,
	ID_PCB_USER_GRID_SETUP,
	ID_PCB_DISPLAY_FOOTPRINT_DOC,
	ID_PCBUNUSED2,
	ID_PCBUNUSED3,
	ID_PCBUNUSED4,
	ID_PCBUNUSED5,
	ID_PCBUNUSED6,
	ID_PCBUNUSED7,

	ID_MODEDIT_CHECK,
	ID_MODEDIT_SAVE_LIBMODULE,
	ID_MODEDIT_NEW_MODULE,
	ID_MODEDIT_SHEET_SET,
	ID_MODEDIT_LOAD_MODULE,
	ID_MODEDIT_ADD_PAD,
	ID_MODEDIT_PLACE_ANCHOR,
	ID_MODEDIT_DELETE_ITEM_BUTT,
	ID_MODEDIT_PAD_SETTINGS,
	ID_MODEDIT_LOAD_MODULE_FROM_BOARD,
	ID_MODEDIT_INSERT_MODULE_IN_BOARD,
	ID_MODEDIT_UPDATE_MODULE_IN_BOARD,
	ID_MODEDIT_EDIT_MODULE_PROPERTIES,
	ID_MODEDIT_TRANSFORM_MODULE,
	ID_MODEDIT_MODULE_ROTATE,
	ID_MODEDIT_MODULE_MIRROR,
	ID_MODEDIT_MODULE_SCALE,
	ID_MODEDIT_MODULE_SCALEX,
	ID_MODEDIT_MODULE_SCALEY,
	ID_MODEDIT_UNDO,
	ID_MODEDIT_REDO,
	ID_MODEDIT_UNUSED6,
	ID_MODEDIT_UNUSED7,
	ID_MODEDIT_UNUSED8,

	// For GERBVIEW only
	ID_GERBVIEW_SHOW_LIST_DCODES,
	ID_GERBVIEW_LOAD_DRILL_FILE,
	ID_GERBVIEW_LOAD_DCODE_FILE,
	ID_TOOLBARH_GERBER_SELECT_TOOL,
	ID_MENU_INC_LAYER_AND_APPEND_FILE,
	ID_INC_LAYER_AND_APPEND_FILE,
	ID_GERBVIEW_SHOW_SOURCE,
	ID_GERBVIEW_EXPORT_TO_PCBNEW,
	ID_GERBVIEW_POPUP_DELETE_DCODE_ITEMS,
	ID_GERBVIEW_UNUSED2,
	ID_GERBVIEW_UNUSED3,
	ID_GERBVIEW_UNUSED4,
	ID_GERBVIEW_UNUSED5,
	ID_GERBVIEW_UNUSED6,
	ID_GERBVIEW_UNUSED7,
	ID_GERBVIEW_UNUSED8,

	////////////////////

	ID_EDA_SOCKET_EVENT_SERV,
	ID_EDA_SOCKET_EVENT,

	ID_TB_OPTIONS_START,

	ID_TB_OPTIONS_DRC_OFF,
	ID_TB_OPTIONS_SELECT_UNIT_MM,
	ID_TB_OPTIONS_SELECT_UNIT_INCH,
	ID_TB_OPTIONS_SELECT_CURSOR,
	ID_TB_OPTIONS_SHOW_POLAR_COORD,
	ID_TB_OPTIONS_SHOW_GRID,
	ID_TB_OPTIONS_SHOW_RATSNEST,
	ID_TB_OPTIONS_SHOW_MODULE_RATSNEST,
	ID_TB_OPTIONS_AUTO_DEL_TRACK,
	ID_TB_OPTIONS_SHOW_ZONES,

	ID_TB_OPTIONS_HIDDEN_PINS,
	ID_TB_OPTIONS_BUS_WIRES_ORIENT,
	ID_TB_OPTIONS_SHOW_PADS_SKETCH,
	ID_TB_OPTIONS_SHOW_TRACKS_SKETCH,
	ID_TB_OPTIONS_SHOW_MODULE_TEXT_SKETCH,
	ID_TB_OPTIONS_SHOW_MODULE_EDGE_SKETCH,
	ID_TB_OPTIONS_SHOW_DCODES,
	ID_TB_OPTIONS_SHOW_HIGHT_CONTRAST_MODE,
	ID_TB_OPTIONS_SHOW_EXTRA_VERTICAL_TOOLBAR1,
	ID_TB_OPTIONS_UNUSED5,
	ID_TB_OPTIONS_UNUSED6,
	ID_TB_OPTIONS_UNUSED7,
	ID_TB_OPTIONS_UNUSED8,

	ID_TB_OPTIONS_END,


	ID_START_COMMAND_3D,
	ID_ROTATE3D_X_NEG,
	ID_ROTATE3D_X_POS,
	ID_ROTATE3D_Y_NEG,
	ID_ROTATE3D_Y_POS,
	ID_ROTATE3D_Z_NEG,
	ID_ROTATE3D_Z_POS,
	ID_RELOAD3D_BOARD,
	ID_TOOL_SCREENCOPY_TOCLIBBOARD,
	ID_MOVE3D_LEFT,
	ID_MOVE3D_RIGHT,
	ID_MOVE3D_UP,
	ID_MOVE3D_DOWN,
	ID_MENU3D_BGCOLOR_SELECTION,
	ID_3D_UNUSED0,
	ID_3D_UNUSED1,
	ID_3D_UNUSED2,
	ID_3D_UNUSED3,
	ID_3D_UNUSED4,
	ID_3D_UNUSED5,
	ID_3D_UNUSED6,
	ID_END_COMMAND_3D,

	ID_MENU_SCREENCOPY_PNG,
	ID_MENU_SCREENCOPY_JPEG,
	ID_MENU_SCREENCOPY_TOCLIBBOARD,
	ID_MENU_COMMAND3D_UNUSED2,
	ID_MENU_COMMAND3D_UNUSED3,
	ID_MENU_COMMAND3D_UNUSED4,
	ID_MENU_COMMAND3D_UNUSED5,
	ID_MENU_COMMAND3D_UNUSED6,


	ID_PCB_MUWAVE_START_CMD,
	ID_PCB_MUWAVE_TOOL_DISPLAY_TOOLS,
	ID_PCB_MUWAVE_TOOL_SELF_CMD,
	ID_PCB_MUWAVE_TOOL_GAP_CMD,
	ID_PCB_MUWAVE_TOOL_STUB_CMD,
	ID_PCB_MUWAVE_TOOL_STUB_ARC_CMD,
	ID_PCB_MUWAVE_TOOL_FUNCTION_SHAPE_CMD,
	ID_PCB_MUWAVE_UNUSED0,
	ID_PCB_MUWAVE_UNUSED1,
	ID_PCB_MUWAVE_UNUSED2,
	ID_PCB_MUWAVE_END_CMD,

	ID_END_LIST
};

#endif	/* define ID_H */
