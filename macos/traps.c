
#include <stddef.h> // null
#include <stdint.h>

const char *TrapName(uint16_t trap)
{

    switch(trap)
    {
/*

    ; QuickDraw

*/

	case 0xA817: return "_CopyMask";
	case 0xA837: return "_MeasureText";
	case 0xA836: return "_GetMaskTable";
	case 0xA838: return "_CalcMask";
	case 0xA839: return "_SeedFill";
	case 0xA850: return "_InitCursor";
	case 0xA851: return "_SetCursor";
	case 0xA852: return "_HideCursor";
	case 0xA853: return "_ShowCursor";
	case 0xA855: return "_ShieldCursor";
	case 0xA856: return "_ObscureCursor";
	case 0xA858: return "_BitAnd";
	case 0xA859: return "_BitXOr";
	case 0xA85A: return "_BitNot";
	case 0xA85B: return "_BitOr";
	case 0xA85C: return "_BitShift";
	case 0xA85D: return "_BitTst";
	case 0xA85E: return "_BitSet";
	case 0xA85F: return "_BitClr";
	case 0xA861: return "_Random";
	case 0xA862: return "_ForeColor";
	case 0xA863: return "_BackColor";
	case 0xA864: return "_ColorBit";
	case 0xA865: return "_GetPixel";
	case 0xA866: return "_StuffHex";
	case 0xA867: return "_LongMul";
	case 0xA868: return "_FixMul";
	case 0xA869: return "_FixRatio";
	case 0xA86A: return "_HiWord";
	case 0xA86B: return "_LoWord";
	case 0xA86C: return "_FixRound";
	case 0xA86D: return "_InitPort";
	case 0xA86E: return "_InitGraf";
	case 0xA86F: return "_OpenPort";
	case 0xA870: return "_LocalToGlobal";
	case 0xA871: return "_GlobalToLocal";
	case 0xA872: return "_GrafDevice";
	case 0xA873: return "_SetPort";
	case 0xA874: return "_GetPort";
	case 0xA875: return "_SetPBits";
	case 0xA876: return "_PortSize";
	case 0xA877: return "_MovePortTo";
	case 0xA878: return "_SetOrigin";
	case 0xA879: return "_SetClip";
	case 0xA87A: return "_GetClip";
	case 0xA87B: return "_ClipRect";
	case 0xA87C: return "_BackPat";
	case 0xA87D: return "_ClosePort";
	case 0xA87E: return "_AddPt";
	case 0xA87F: return "_SubPt";
	case 0xA880: return "_SetPt";
	case 0xA881: return "_EqualPt";
	case 0xA882: return "_StdText";
	case 0xA883: return "_DrawChar";
	case 0xA884: return "_DrawString";
	case 0xA885: return "_DrawText";
	case 0xA886: return "_TextWidth";
	case 0xA887: return "_TextFont";
	case 0xA888: return "_TextFace";
	case 0xA889: return "_TextMode";
	case 0xA88A: return "_TextSize";
	case 0xA88B: return "_GetFontInfo";
	case 0xA88C: return "_StringWidth";
	case 0xA88D: return "_CharWidth";
	case 0xA88E: return "_SpaceExtra";
	case 0xA890: return "_StdLine";
	case 0xA891: return "_LineTo";
	case 0xA892: return "_Line";
	case 0xA893: return "_MoveTo";
	case 0xA894: return "_Move";
	case 0xA895: return "_ShutDown";
	case 0xA896: return "_HidePen";
	case 0xA897: return "_ShowPen";
	case 0xA898: return "_GetPenState";
	case 0xA899: return "_SetPenState";
	case 0xA89A: return "_GetPen";
	case 0xA89B: return "_PenSize";
	case 0xA89C: return "_PenMode";
	case 0xA89D: return "_PenPat";
	case 0xA89E: return "_PenNormal";
	case 0xA89F: return "_Unimplemented";
	case 0xA8A0: return "_StdRect";
	case 0xA8A1: return "_FrameRect";
	case 0xA8A2: return "_PaintRect";
	case 0xA8A3: return "_EraseRect";
	case 0xA8A4: return "_InverRect";
	case 0xA8A5: return "_FillRect";
	case 0xA8A6: return "_EqualRect";
	case 0xA8A7: return "_SetRect";
	case 0xA8A8: return "_OffsetRect";
//	case 0xA8A8: return "_OffSetRect";
	case 0xA8A9: return "_InsetRect";
//	case 0xA8A9: return "_InSetRect";
	case 0xA8AA: return "_SectRect";
	case 0xA8AB: return "_UnionRect";
	case 0xA8AC: return "_Pt2Rect";
	case 0xA8AD: return "_PtInRect";
	case 0xA8AE: return "_EmptyRect";
	case 0xA8AF: return "_StdRRect";
	case 0xA8B0: return "_FrameRoundRect";
	case 0xA8B1: return "_PaintRoundRect";
	case 0xA8B2: return "_EraseRoundRect";
	case 0xA8B3: return "_InverRoundRect";
	case 0xA8B4: return "_FillRoundRect";
	case 0xA8B6: return "_StdOval";
	case 0xA8B7: return "_FrameOval";
	case 0xA8B8: return "_PaintOval";
	case 0xA8B9: return "_EraseOval";
	case 0xA8BA: return "_InvertOval";
	case 0xA8BB: return "_FillOval";
	case 0xA8BC: return "_SlopeFromAngle";
	case 0xA8BD: return "_StdArc";
	case 0xA8BE: return "_FrameArc";
	case 0xA8BF: return "_PaintArc";
	case 0xA8C0: return "_EraseArc";
	case 0xA8C1: return "_InvertArc";
	case 0xA8C2: return "_FillArc";
	case 0xA8C3: return "_PtToAngle";
	case 0xA8C4: return "_AngleFromSlope";
	case 0xA8C5: return "_StdPoly";
	case 0xA8C6: return "_FramePoly";
	case 0xA8C7: return "_PaintPoly";
	case 0xA8C8: return "_ErasePoly";
	case 0xA8C9: return "_InvertPoly";
	case 0xA8CA: return "_FillPoly";
	case 0xA8CB: return "_OpenPoly";
//	case 0xA8CC: return "_ClosePgon";
	case 0xA8CC: return "_ClosePoly";
	case 0xA8CD: return "_KillPoly";
	case 0xA8CE: return "_OffsetPoly";
//	case 0xA8CE: return "_OffSetPoly";
	case 0xA8CF: return "_PackBits";
	case 0xA8D0: return "_UnpackBits";
	case 0xA8D1: return "_StdRgn";
	case 0xA8D2: return "_FrameRgn";
	case 0xA8D3: return "_PaintRgn";
	case 0xA8D4: return "_EraseRgn";
	case 0xA8D5: return "_InverRgn";
	case 0xA8D6: return "_FillRgn";
//	case 0xA8D7: return "_BitMapRgn";
	case 0xA8D7: return "_BitMapToRegion";
	case 0xA8D8: return "_NewRgn";
//	case 0xA8D9: return "_DisposRgn";
	case 0xA8D9: return "_DisposeRgn";
	case 0xA8DA: return "_OpenRgn";
	case 0xA8DB: return "_CloseRgn";
	case 0xA8DC: return "_CopyRgn";
	case 0xA8DD: return "_SetEmptyRgn";
	case 0xA8DE: return "_SetRecRgn";
	case 0xA8DF: return "_RectRgn";
//	case 0xA8E0: return "_OfsetRgn";
//	case 0xA8E0: return "_OfSetRgn";
	case 0xA8E0: return "_OffsetRgn";
	case 0xA8E1: return "_InsetRgn";
//	case 0xA8E1: return "_InSetRgn";
	case 0xA8E2: return "_EmptyRgn";
	case 0xA8E3: return "_EqualRgn";
	case 0xA8E4: return "_SectRgn";
	case 0xA8E5: return "_UnionRgn";
	case 0xA8E6: return "_DiffRgn";
	case 0xA8E7: return "_XOrRgn";
	case 0xA8E8: return "_PtInRgn";
	case 0xA8E9: return "_RectInRgn";
	case 0xA8EA: return "_SetStdProcs";
	case 0xA8EB: return "_StdBits";
	case 0xA8EC: return "_CopyBits";
	case 0xA8ED: return "_StdTxMeas";
	case 0xA8EE: return "_StdGetPic";
	case 0xA8EF: return "_ScrollRect";
	case 0xA8F0: return "_StdPutPic";
	case 0xA8F1: return "_StdComment";
	case 0xA8F2: return "_PicComment";
	case 0xA8F3: return "_OpenPicture";
	case 0xA8F4: return "_ClosePicture";
	case 0xA8F5: return "_KillPicture";
	case 0xA8F6: return "_DrawPicture";
	case 0xA8F7: return "_Layout";
	case 0xA8F8: return "_ScalePt";
	case 0xA8F9: return "_MapPt";
	case 0xA8FA: return "_MapRect";
	case 0xA8FB: return "_MapRgn";
	case 0xA8FC: return "_MapPoly";

/*

    ; Toolbox

*/

	case 0xA80D: return "_Count1Resources";
	case 0xA80E: return "_Get1IxResource";
	case 0xA80F: return "_Get1IxType";
	case 0xA810: return "_Unique1ID";
	case 0xA811: return "_TESelView";
	case 0xA812: return "_TEPinScroll";
	case 0xA813: return "_TEAutoView";
	case 0xA816: return "_Pack8";
	case 0xA818: return "_FixATan2";
	case 0xA819: return "_XMunger";
	case 0xA81A: return "_HOpenResFile";
	case 0xA81B: return "_HCreateResFile";
	case 0xA81C: return "_Count1Types";
	case 0xA81F: return "_Get1Resource";
	case 0xA820: return "_Get1NamedResource";
	case 0xA821: return "_MaxSizeRsrc";
//	case 0xA826: return "_InsMenuItem";
	case 0xA826: return "_InsertMenuItem";
//	case 0xA827: return "_HideDItem";
	case 0xA827: return "_HideDialogItem";
//	case 0xA828: return "_ShowDItem";
	case 0xA828: return "_ShowDialogItem";
	case 0xA829: return "_LayerDispatch";
	case 0xA82B: return "_Pack9";
	case 0xA82C: return "_Pack10";
	case 0xA82D: return "_Pack11";
	case 0xA82E: return "_Pack12";
	case 0xA82F: return "_Pack13";
	case 0xA830: return "_Pack14";
	case 0xA831: return "_Pack15";
	case 0xA833: return "_ScrnBitMap";
	case 0xA834: return "_SetFScaleDisable";
	case 0xA835: return "_FontMetrics";
	case 0xA83A: return "_ZoomWindow";
	case 0xA83B: return "_TrackBox";
	case 0xA8FD: return "_PrGlue";
	case 0xA8FE: return "_InitFonts";
	case 0xA8FF: return "_GetFName";
	case 0xA900: return "_GetFNum";
	case 0xA901: return "_FMSwapFont";
	case 0xA902: return "_RealFont";
	case 0xA903: return "_SetFontLock";
	case 0xA904: return "_DrawGrowIcon";
	case 0xA905: return "_DragGrayRgn";
	case 0xA906: return "_NewString";
	case 0xA907: return "_SetString";
	case 0xA908: return "_ShowHide";
	case 0xA909: return "_CalcVis";
	case 0xA90A: return "_CalcVBehind";
	case 0xA90B: return "_ClipAbove";
	case 0xA90C: return "_PaintOne";
	case 0xA90D: return "_PaintBehind";
	case 0xA90E: return "_SaveOld";
	case 0xA90F: return "_DrawNew";
	case 0xA910: return "_GetWMgrPort";
	case 0xA911: return "_CheckUpDate";
	case 0xA912: return "_InitWindows";
	case 0xA913: return "_NewWindow";
//	case 0xA914: return "_DisposWindow";
	case 0xA914: return "_DisposeWindow";
	case 0xA915: return "_ShowWindow";
	case 0xA916: return "_HideWindow";
	case 0xA917: return "_GetWRefCon";
	case 0xA918: return "_SetWRefCon";
	case 0xA919: return "_GetWTitle";
	case 0xA91A: return "_SetWTitle";
	case 0xA91B: return "_MoveWindow";
	case 0xA91C: return "_HiliteWindow";
	case 0xA91D: return "_SizeWindow";
	case 0xA91E: return "_TrackGoAway";
	case 0xA91F: return "_SelectWindow";
	case 0xA920: return "_BringToFront";
	case 0xA921: return "_SendBehind";
	case 0xA922: return "_BeginUpDate";
	case 0xA923: return "_EndUpDate";
	case 0xA924: return "_FrontWindow";
	case 0xA925: return "_DragWindow";
	case 0xA926: return "_DragTheRgn";
	case 0xA927: return "_InvalRgn";
	case 0xA928: return "_InvalRect";
	case 0xA929: return "_ValidRgn";
	case 0xA92A: return "_ValidRect";
	case 0xA92B: return "_GrowWindow";
	case 0xA92C: return "_FindWindow";
	case 0xA92D: return "_CloseWindow";
	case 0xA92E: return "_SetWindowPic";
	case 0xA92F: return "_GetWindowPic";
	case 0xA930: return "_InitMenus";
	case 0xA931: return "_NewMenu";
//	case 0xA932: return "_DisposMenu";
	case 0xA932: return "_DisposeMenu";
	case 0xA933: return "_AppendMenu";
	case 0xA934: return "_ClearMenuBar";
	case 0xA935: return "_InsertMenu";
	case 0xA936: return "_DeleteMenu";
	case 0xA937: return "_DrawMenuBar";
	case 0xA81D: return "_InvalMenuBar";
	case 0xA938: return "_HiliteMenu";
	case 0xA939: return "_EnableItem";
	case 0xA93A: return "_DisableItem";
	case 0xA93B: return "_GetMenuBar";
	case 0xA93C: return "_SetMenuBar";
	case 0xA93D: return "_MenuSelect";
	case 0xA93E: return "_MenuKey";
	case 0xA93F: return "_GetItmIcon";
	case 0xA940: return "_SetItmIcon";
	case 0xA941: return "_GetItmStyle";
	case 0xA942: return "_SetItmStyle";
	case 0xA943: return "_GetItmMark";
	case 0xA944: return "_SetItmMark";
	case 0xA945: return "_CheckItem";
//	case 0xA946: return "_GetItem";
	case 0xA946: return "_GetMenuItemText";
//	case 0xA947: return "_SetItem";
	case 0xA947: return "_SetMenuItemText";
	case 0xA948: return "_CalcMenuSize";
//	case 0xA949: return "_GetMHandle";
	case 0xA949: return "_GetMenuHandle";
	case 0xA94A: return "_SetMFlash";
	case 0xA94B: return "_PlotIcon";
	case 0xA94C: return "_FlashMenuBar";
//	case 0xA94D: return "_AddResMenu";
	case 0xA94D: return "_AppendResMenu";
	case 0xA94E: return "_PinRect";
	case 0xA94F: return "_DeltaPoint";
	case 0xA950: return "_CountMItems";
	case 0xA951: return "_InsertResMenu";
//	case 0xA952: return "_DelMenuItem";
	case 0xA952: return "_DeleteMenuItem";
	case 0xA953: return "_UpdtControl";
	case 0xA954: return "_NewControl";
//	case 0xA955: return "_DisposControl";
	case 0xA955: return "_DisposeControl";
	case 0xA956: return "_KillControls";
	case 0xA957: return "_ShowControl";
	case 0xA958: return "_HideControl";
	case 0xA959: return "_MoveControl";
//	case 0xA95A: return "_GetCRefCon";
	case 0xA95A: return "_GetControlReference";
//	case 0xA95B: return "_SetCRefCon";
	case 0xA95B: return "_SetControlReference";
	case 0xA95C: return "_SizeControl";
	case 0xA95D: return "_HiliteControl";
//	case 0xA95E: return "_GetCTitle";
	case 0xA95E: return "_GetControlTitle";
//	case 0xA95F: return "_SetCTitle";
	case 0xA95F: return "_SetControlTitle";
//	case 0xA960: return "_GetCtlValue";
	case 0xA960: return "_GetControlValue";
//	case 0xA961: return "_GetMinCtl";
	case 0xA961: return "_GetControlMinimum";
//	case 0xA962: return "_GetMaxCtl";
	case 0xA962: return "_GetControlMaximum";
//	case 0xA963: return "_SetCtlValue";
	case 0xA963: return "_SetControlValue";
//	case 0xA964: return "_SetMinCtl";
	case 0xA964: return "_SetControlMinimum";
//	case 0xA965: return "_SetMaxCtl";
	case 0xA965: return "_SetControlMaximum";
	case 0xA966: return "_TestControl";
	case 0xA967: return "_DragControl";
	case 0xA968: return "_TrackControl";
	case 0xA969: return "_DrawControls";
//	case 0xA96A: return "_GetCtlAction";
	case 0xA96A: return "_GetControlAction";
//	case 0xA96B: return "_SetCtlAction";
	case 0xA96B: return "_SetControlAction";
	case 0xA96C: return "_FindControl";
	case 0xA96D: return "_Draw1Control";
	case 0xA96E: return "_Dequeue";
	case 0xA96F: return "_Enqueue";
	case 0xA860: return "_WaitNextEvent";
	case 0xA970: return "_GetNextEvent";
	case 0xA971: return "_EventAvail";
	case 0xA972: return "_GetMouse";
	case 0xA973: return "_StillDown";
	case 0xA974: return "_Button";
	case 0xA975: return "_TickCount";
	case 0xA976: return "_GetKeys";
	case 0xA977: return "_WaitMouseUp";
	case 0xA978: return "_UpdtDialog";
	case 0xA97B: return "_InitDialogs";
	case 0xA97C: return "_GetNewDialog";
	case 0xA97D: return "_NewDialog";
//	case 0xA97E: return "_SelIText";
	case 0xA97E: return "_SelectDialogItemText";
	case 0xA97F: return "_IsDialogEvent";
	case 0xA980: return "_DialogSelect";
	case 0xA981: return "_DrawDialog";
	case 0xA982: return "_CloseDialog";
//	case 0xA983: return "_DisposDialog";
	case 0xA983: return "_DisposeDialog";
//	case 0xA984: return "_FindDItem";
	case 0xA984: return "_FindDialogItem";
	case 0xA985: return "_Alert";
	case 0xA986: return "_StopAlert";
	case 0xA987: return "_NoteAlert";
	case 0xA988: return "_CautionAlert";
	case 0xA98B: return "_ParamText";
	case 0xA98C: return "_ErrorSound";
//	case 0xA98D: return "_GetDItem";
	case 0xA98D: return "_GetDialogItem";
//	case 0xA98E: return "_SetDItem";
	case 0xA98E: return "_SetDialogItem";
//	case 0xA98F: return "_SetIText";
	case 0xA98F: return "_SetDialogItemText";
//	case 0xA990: return "_GetIText";
	case 0xA990: return "_GetDialogItemText";
	case 0xA991: return "_ModalDialog";
	case 0xA992: return "_DetachResource";
	case 0xA993: return "_SetResPurge";
	case 0xA994: return "_CurResFile";
	case 0xA995: return "_InitResources";
	case 0xA996: return "_RsrcZoneInit";
	case 0xA997: return "_OpenResFile";
	case 0xA998: return "_UseResFile";
	case 0xA999: return "_UpdateResFile";
	case 0xA99A: return "_CloseResFile";
	case 0xA99B: return "_SetResLoad";
	case 0xA99C: return "_CountResources";
	case 0xA99D: return "_GetIndResource";
	case 0xA99E: return "_CountTypes";
	case 0xA99F: return "_GetIndType";
	case 0xA9A0: return "_GetResource";
	case 0xA9A1: return "_GetNamedResource";
	case 0xA9A2: return "_LoadResource";
	case 0xA9A3: return "_ReleaseResource";
	case 0xA9A4: return "_HomeResFile";
	case 0xA9A5: return "_SizeRsrc";
	case 0xA9A6: return "_GetResAttrs";
	case 0xA9A7: return "_SetResAttrs";
	case 0xA9A8: return "_GetResInfo";
	case 0xA9A9: return "_SetResInfo";
	case 0xA9AA: return "_ChangedResource";
	case 0xA9AB: return "_AddResource";
	case 0xA9AC: return "_AddReference";
	case 0xA9AD: return "_RmveResource";
	case 0xA9AE: return "_RmveReference";
	case 0xA9AF: return "_ResError";
	case 0xA9B0: return "_WriteResource";
	case 0xA9B1: return "_CreateResFile";
	case 0xA9B2: return "_SystemEvent";
	case 0xA9B3: return "_SystemClick";
	case 0xA9B4: return "_SystemTask";
	case 0xA9B5: return "_SystemMenu";
	case 0xA9B6: return "_OpenDeskAcc";
	case 0xA9B7: return "_CloseDeskAcc";
	case 0xA9B8: return "_GetPattern";
	case 0xA9B9: return "_GetCursor";
	case 0xA9BA: return "_GetString";
	case 0xA9BB: return "_GetIcon";
	case 0xA9BC: return "_GetPicture";
	case 0xA9BD: return "_GetNewWindow";
	case 0xA9BE: return "_GetNewControl";
	case 0xA9BF: return "_GetRMenu";
	case 0xA9C0: return "_GetNewMBar";
	case 0xA9C1: return "_UniqueID";
	case 0xA9C2: return "_SysEdit";
	case 0xA9C4: return "_OpenRFPerm";
	case 0xA9C5: return "_RsrcMapEntry";
//	case 0xA9C6: return "_Secs2Date";
	case 0xA9C6: return "_SecondsToDate";
//	case 0xA9C7: return "_Date2Secs";
	case 0xA9C7: return "_DateToSeconds";
	case 0xA9C8: return "_SysBeep";
	case 0xA9C9: return "_SysError";
	case 0xA9CA: return "_PutIcon";
	case 0xA9E0: return "_Munger";
	case 0xA9E1: return "_HandToHand";
	case 0xA9E2: return "_PtrToXHand";
	case 0xA9E3: return "_PtrToHand";
	case 0xA9E4: return "_HandAndHand";
	case 0xA9E5: return "_InitPack";
	case 0xA9E6: return "_InitAllPacks";
	case 0xA9E7: return "_Pack0";
	case 0xA9E8: return "_Pack1";
	case 0xA9E9: return "_Pack2";
	case 0xA9EA: return "_Pack3";
	case 0xA9EB: return "_FP68K";
//	case 0xA9EB: return "_Pack4";
	case 0xA9EC: return "_Elems68K";
//	case 0xA9EC: return "_Pack5";
	case 0xA9ED: return "_Pack6";
	case 0xA9EE: return "_DECSTR68K";
//	case 0xA9EE: return "_Pack7";
	case 0xA9EF: return "_PtrAndHand";
	case 0xA9F0: return "_LoadSeg";
	case 0xA9F1: return "_UnLoadSeg";
	case 0xA9F2: return "_Launch";
	case 0xA9F3: return "_Chain";
	case 0xA9F4: return "_ExitToShell";
	case 0xA9F5: return "_GetAppParms";
	case 0xA9F6: return "_GetResFileAttrs";
	case 0xA9F7: return "_SetResFileAttrs";
	case 0xA9F8: return "_MethodDispatch";
	case 0xA9F9: return "_InfoScrap";
//	case 0xA9FA: return "_UnlodeScrap";
	case 0xA9FA: return "_UnloadScrap";
//	case 0xA9FB: return "_LodeScrap";
	case 0xA9FB: return "_LoadScrap";
	case 0xA9FC: return "_ZeroScrap";
	case 0xA9FD: return "_GetScrap";
	case 0xA9FE: return "_PutScrap";
	case 0xA9FF: return "_Debugger";
	case 0xABEB: return "_DisplayDispatch";
	case 0xABC9: return "_IconDispatch";
	case 0xABF2: return "_ThreadDispatch";
	case 0xABFF: return "_DebugStr";

/*

    ; Resource Manager

*/

	case 0xA822: return "_ResourceDispatch";

/*

    ; PPCToolbox

*/

	case 0xA0DD: return "_PPC";

/*

    ; Alias Manager

*/

	case 0xA823: return "_AliasDispatch";

/*

    ; Device Manager (some shared by the File Manager)

*/

	case 0xA000: return "_Open";
	case 0xA001: return "_Close";
	case 0xA002: return "_Read";
	case 0xA003: return "_Write";
	case 0xA004: return "_Control";
	case 0xA005: return "_Status";
	case 0xA006: return "_KillIO";

/*

    ; File Manager

*/

	case 0xA007: return "_GetVolInfo";
	case 0xA008: return "_Create";
	case 0xA009: return "_Delete";
	case 0xA00A: return "_OpenRF";
	case 0xA00B: return "_Rename";
//	case 0xA00B: return "_ReName";
	case 0xA00C: return "_GetFileInfo";
	case 0xA00D: return "_SetFileInfo";
	case 0xA00E: return "_UnmountVol";
//	case 0xA00E: return "_UnMountVol";
	case 0xA20E: return "_HUnmountVol";
	case 0xA00F: return "_MountVol";
	case 0xA010: return "_Allocate";
	case 0xA011: return "_GetEOF";
	case 0xA012: return "_SetEOF";
	case 0xA013: return "_FlushVol";
	case 0xA014: return "_GetVol";
	case 0xA015: return "_SetVol";
	case 0xA016: return "_FInitQueue";
	case 0xA017: return "_Eject";
	case 0xA018: return "_GetFPos";
	case 0xA041: return "_SetFilLock";
	case 0xA042: return "_RstFilLock";
	case 0xA043: return "_SetFilType";
	case 0xA044: return "_SetFPos";
	case 0xA045: return "_FlushFile";
	case 0xA200: return "_HOpen";
	case 0xA207: return "_HGetVInfo";
	case 0xA208: return "_HCreate";
	case 0xA209: return "_HDelete";
	case 0xA20A: return "_HOpenRF";
	case 0xA20B: return "_HRename";
	case 0xA20C: return "_HGetFileInfo";
	case 0xA20D: return "_HSetFileInfo";
	case 0xA210: return "_AllocContig";
	case 0xA215: return "_HSetVol";
	case 0xA214: return "_HGetVol";
	case 0xA241: return "_HSetFLock";
	case 0xA242: return "_HRstFLock";

/*

    ; dispatch trap for remaining File Manager (and Desktop Manager) calls

*/

	case 0xA060: return "_FSDispatch";
	case 0xA260: return "_HFSDispatch";

/*

    ; High level FSSpec calls

*/

	case 0xAA52: return "_HighLevelHFSDispatch";

/*

    ; Memory Manager

*/

	case 0xA019: return "_InitZone";
	case 0xA11A: return "_GetZone";
	case 0xA01B: return "_SetZone";
	case 0xA01C: return "_FreeMem";
	case 0xA11D: return "_MaxMem";
	case 0xA11E: return "_NewPtr";
	case 0xA51E: return "_NewPtrSys";
	case 0xA31E: return "_NewPtrClear";
	case 0xA71E: return "_NewPtrSysClear";
//	case 0xA01F: return "_DisposPtr";
	case 0xA01F: return "_DisposePtr";
	case 0xA020: return "_SetPtrSize";
	case 0xA021: return "_GetPtrSize";
	case 0xA122: return "_NewHandle";
	case 0xA322: return "_NewHandleClear";
//	case 0xA023: return "_DisposHandle";
	case 0xA023: return "_DisposeHandle";
	case 0xA024: return "_SetHandleSize";
	case 0xA025: return "_GetHandleSize";
	case 0xA126: return "_HandleZone";
	case 0xA027: return "_ReallocHandle";
	case 0xA128: return "_RecoverHandle";
	case 0xA029: return "_HLock";
	case 0xA02A: return "_HUnlock";
	case 0xA02B: return "_EmptyHandle";
	case 0xA02C: return "_InitApplZone";
	case 0xA02D: return "_SetApplLimit";
	case 0xA02E: return "_BlockMove";
	case 0xA22E: return "_BlockMoveData";
	case 0xA05C: return "_MemoryDispatch";
	case 0xA15C: return "_MemoryDispatchA0Result";
	case 0xA08F: return "_DeferUserFn";
	case 0xA08D: return "_DebugUtil";

/*

    ; Event Manager

*/

	case 0xA02F: return "_PostEvent";
	case 0xA12F: return "_PPostEvent";
	case 0xA030: return "_OSEventAvail";
	case 0xA031: return "_GetOSEvent";
	case 0xA032: return "_FlushEvents";
	case 0xA033: return "_VInstall";
	case 0xA034: return "_VRemove";
	case 0xA035: return "_OffLine";
	case 0xA036: return "_MoreMasters";
	case 0xA038: return "_WriteParam";
	case 0xA039: return "_ReadDateTime";
	case 0xA03A: return "_SetDateTime";
	case 0xA03B: return "_Delay";
	case 0xA03C: return "_CmpString";
	case 0xA03D: return "_DrvrInstall";
	case 0xA03E: return "_DrvrRemove";
	case 0xA03F: return "_InitUtil";
	case 0xA040: return "_ResrvMem";
	case 0xA146: return "_GetTrapAddress";
	case 0xA047: return "_SetTrapAddress";
	case 0xA346: return "_GetOSTrapAddress";
	case 0xA247: return "_SetOSTrapAddress";
	case 0xA746: return "_GetToolTrapAddress";
	case 0xA647: return "_SetToolTrapAddress";
//	case 0xA746: return "_GetToolBoxTrapAddress";
//	case 0xA647: return "_SetToolBoxTrapAddress";
	case 0xA148: return "_PtrZone";
	case 0xA049: return "_HPurge";
	case 0xA04A: return "_HNoPurge";
	case 0xA04B: return "_SetGrowZone";
	case 0xA04C: return "_CompactMem";
	case 0xA04D: return "_PurgeMem";
	case 0xA04E: return "_AddDrive";
	case 0xA04F: return "_RDrvrInstall";
//	case 0xA056: return "_LwrString";
	case 0xA054: return "_UprString";
//	case 0xA057: return "_SetApplBase";
	case 0xA198: return "_HWPriv";

/*

    ; New names for (mostly) new flavors of old LwrString trap (redone <13>)
*/

	case 0xA056: return "_LowerText";
	case 0xA256: return "_StripText";
	case 0xA456: return "_UpperText";
	case 0xA656: return "_StripUpperText";

/*

    ; Temporary Memory routines

*/

	case 0xA88F: return "_OSDispatch";
//	case 0xA050: return "_RelString";
	case 0xA050: return "_CompareString";
	case 0xA051: return "_ReadXPRam";
	case 0xA052: return "_WriteXPRam";
	case 0xA058: return "_InsTime";
	case 0xA458: return "_InsXTime";
	case 0xA059: return "_RmvTime";
	case 0xA05A: return "_PrimeTime";
	case 0xA05B: return "_PowerOff";
	case 0xA061: return "_MaxBlock";
	case 0xA162: return "_PurgeSpace";
	case 0xA562: return "_PurgeSpaceSys";
	case 0xA063: return "_MaxApplZone";
	case 0xA064: return "_MoveHHi";
	case 0xA065: return "_StackSpace";
	case 0xA166: return "_NewEmptyHandle";
	case 0xA067: return "_HSetRBit";
	case 0xA068: return "_HClrRBit";
	case 0xA069: return "_HGetState";
	case 0xA06A: return "_HSetState";
	case 0xA06C: return "_InitFS";
	case 0xA06D: return "_InitEvents";
	case 0xA055: return "_StripAddress";
	case 0xA091: return "_Translate24To32";
	case 0xA057: return "_SetAppBase";
	case 0xA05D: return "_SwapMMUMode";
	case 0xA06F: return "_SlotVInstall";
	case 0xA070: return "_SlotVRemove";
	case 0xA071: return "_AttachVBL";
	case 0xA072: return "_DoVBLTask";
	case 0xA075: return "_SIntInstall";
	case 0xA076: return "_SIntRemove";
	case 0xA077: return "_CountADBs";
	case 0xA078: return "_GetIndADB";
	case 0xA079: return "_GetADBInfo";
	case 0xA07A: return "_SetADBInfo";
	case 0xA07B: return "_ADBReInit";
	case 0xA07C: return "_ADBOp";
	case 0xA07D: return "_GetDefaultStartup";
	case 0xA07E: return "_SetDefaultStartup";
	case 0xA07F: return "_InternalWait";
	case 0xA80C: return "_RGetResource";
	case 0xA080: return "_GetVideoDefault";
	case 0xA081: return "_SetVideoDefault";
	case 0xA082: return "_DTInstall";
	case 0xA083: return "_SetOSDefault";
	case 0xA084: return "_GetOSDefault";
	case 0xA086: return "_IOPInfoAccess";
	case 0xA087: return "_IOPMsgRequest";
	case 0xA088: return "_IOPMoveData";

/*

    ; Power Manager

*/

	case 0xA09F: return "_PowerDispatch";
	case 0xA085: return "_PMgrOp";
	case 0xA285: return "_IdleUpdate";
	case 0xA485: return "_IdleState";
	case 0xA685: return "_SerialPower";
	case 0xA08A: return "_Sleep";
	case 0xA28A: return "_SleepQInstall";
//	case 0xA28A: return "_SlpQInstall";
	case 0xA48A: return "_SleepQRemove";
//	case 0xA48A: return "_SlpQRemove";

/*

    ; Comm. Toolbox

*/

	case 0xA08B: return "_CommToolboxDispatch";
	case 0xA090: return "_SysEnvirons";

/*

    ; Egret Manager

*/

	case 0xA092: return "_EgretDispatch";
	case 0xA1AD: return "_Gestalt";
	case 0xA3AD: return "_NewGestalt";
	case 0xA5AD: return "_ReplaceGestalt";
	case 0xA7AD: return "_GetGestaltProcPtr";
	case 0xA808: return "_InitProcMenu";
	case 0xA84E: return "_GetItemCmd";
	case 0xA84F: return "_SetItemCmd";
	case 0xA80B: return "_PopUpMenuSelect";
//	case 0xA9C3: return "_KeyTrans";
	case 0xA9C3: return "_KeyTranslate";

/*

    ; TextEdit

*/

	case 0xA9CB: return "_TEGetText";
	case 0xA9CC: return "_TEInit";
	case 0xA9CD: return "_TEDispose";
//	case 0xA9CE: return "_TextBox";
	case 0xA9CE: return "_TETextBox";
	case 0xA9CF: return "_TESetText";
	case 0xA9D0: return "_TECalText";
	case 0xA9D1: return "_TESetSelect";
	case 0xA9D2: return "_TENew";
	case 0xA9D3: return "_TEUpdate";
	case 0xA9D4: return "_TEClick";
	case 0xA9D5: return "_TECopy";
	case 0xA9D6: return "_TECut";
	case 0xA9D7: return "_TEDelete";
	case 0xA9D8: return "_TEActivate";
	case 0xA9D9: return "_TEDeactivate";
	case 0xA9DA: return "_TEIdle";
	case 0xA9DB: return "_TEPaste";
	case 0xA9DC: return "_TEKey";
	case 0xA9DD: return "_TEScroll";
	case 0xA9DE: return "_TEInsert";
//	case 0xA9DF: return "_TESetJust";
	case 0xA9DF: return "_TESetAlignment";
	case 0xA83C: return "_TEGetOffset";
	case 0xA83D: return "_TEDispatch";
	case 0xA83E: return "_TEStyleNew";

/*

    ; Color Quickdraw

*/

	case 0xAA00: return "_OpenCPort";
//	case 0xAA00: return "_OpenCport";
	case 0xAA01: return "_InitCPort";
//	case 0xAA01: return "_InitCport";
//	case 0xA87D: return "_CloseCPort";
//	case 0xA87D: return "_CloseCport";
	case 0xAA03: return "_NewPixMap";
//	case 0xAA04: return "_DisposPixMap";
	case 0xAA04: return "_DisposePixMap";
	case 0xAA05: return "_CopyPixMap";
	case 0xAA06: return "_SetPortPix";
	case 0xAA07: return "_NewPixPat";
//	case 0xAA08: return "_DisposPixPat";
	case 0xAA08: return "_DisposePixPat";
	case 0xAA09: return "_CopyPixPat";
	case 0xAA0A: return "_PenPixPat";
	case 0xAA0B: return "_BackPixPat";
	case 0xAA0C: return "_GetPixPat";
	case 0xAA0D: return "_MakeRGBPat";
	case 0xAA0E: return "_FillCRect";
	case 0xAA0F: return "_FillCOval";
	case 0xAA10: return "_FillCRoundRect";
	case 0xAA11: return "_FillCArc";
	case 0xAA12: return "_FillCRgn";
	case 0xAA13: return "_FillCPoly";
	case 0xAA14: return "_RGBForeColor";
	case 0xAA15: return "_RGBBackColor";
	case 0xAA16: return "_SetCPixel";
	case 0xAA17: return "_GetCPixel";
	case 0xAA18: return "_GetCTable";
	case 0xAA19: return "_GetForeColor";
	case 0xAA1A: return "_GetBackColor";
	case 0xAA1B: return "_GetCCursor";
	case 0xAA1C: return "_SetCCursor";
	case 0xAA1D: return "_AllocCursor";
	case 0xAA1E: return "_GetCIcon";
	case 0xAA1F: return "_PlotCIcon";
	case 0xAA20: return "_OpenCPicture";
	case 0xAA21: return "_OpColor";
	case 0xAA22: return "_HiliteColor";
	case 0xAA23: return "_CharExtra";
//	case 0xAA24: return "_DisposCTable";
	case 0xAA24: return "_DisposeCTable";
//	case 0xAA25: return "_DisposCIcon";
	case 0xAA25: return "_DisposeCIcon";
//	case 0xAA26: return "_DisposCCursor";
	case 0xAA26: return "_DisposeCCursor";
	case 0xAA50: return "_SeedCFill";
	case 0xAA4F: return "_CalcCMask";
	case 0xAA51: return "_CopyDeepMask";

/*

    ; Routines for video devices

*/

	case 0xAA27: return "_GetMaxDevice";
	case 0xAA28: return "_GetCTSeed";
	case 0xAA29: return "_GetDeviceList";
	case 0xAA2A: return "_GetMainDevice";
	case 0xAA2B: return "_GetNextDevice";
	case 0xAA2C: return "_TestDeviceAttribute";
	case 0xAA2D: return "_SetDeviceAttribute";
	case 0xAA2E: return "_InitGDevice";
	case 0xAA2F: return "_NewGDevice";
//	case 0xAA30: return "_DisposGDevice";
	case 0xAA30: return "_DisposeGDevice";
	case 0xAA31: return "_SetGDevice";
	case 0xAA32: return "_GetGDevice";
	case 0xABCA: return "_DeviceLoop";

/*

    ; Color Manager

*/

	case 0xAA33: return "_Color2Index";
	case 0xAA34: return "_Index2Color";
	case 0xAA35: return "_InvertColor";
	case 0xAA36: return "_RealColor";
	case 0xAA37: return "_GetSubTable";
	case 0xAA38: return "_UpdatePixMap";

/*

    ; Dialog Manager

*/

//	case 0xAA4B: return "_NewCDialog";
	case 0xAA4B: return "_NewColorDialog";
	case 0xAA39: return "_MakeITable";
	case 0xAA3A: return "_AddSearch";
	case 0xAA3B: return "_AddComp";
	case 0xAA3C: return "_SetClientID";
	case 0xAA3D: return "_ProtectEntry";
	case 0xAA3E: return "_ReserveEntry";
	case 0xAA3F: return "_SetEntries";
	case 0xAA40: return "_QDError";
	case 0xAA49: return "_SaveEntries";
	case 0xAA4A: return "_RestoreEntries";
	case 0xAA4C: return "_DelSearch";
	case 0xAA4D: return "_DelComp";
	case 0xAA4E: return "_SetStdCProcs";
	case 0xABF8: return "_StdOpcodeProc";

/*

    ; added to Toolbox for color

*/

	case 0xAA41: return "_SetWinColor";
	case 0xAA42: return "_GetAuxWin";
//	case 0xAA43: return "_SetCtlColor";
	case 0xAA43: return "_SetControlColor";
//	case 0xAA44: return "_GetAuxCtl";
	case 0xAA44: return "_GetAuxiliaryControlRecord";
	case 0xAA45: return "_NewCWindow";
	case 0xAA46: return "_GetNewCWindow";
	case 0xAA47: return "_SetDeskCPat";
	case 0xAA48: return "_GetCWMgrPort";
//	case 0xA809: return "_GetCVariant";
	case 0xA809: return "_GetControlVariant";
	case 0xA80A: return "_GetWVariant";

/*

    ; added to Menu Manager for color

*/

//	case 0xAA60: return "_DelMCEntries";
	case 0xAA60: return "_DeleteMCEntries";
	case 0xAA61: return "_GetMCInfo";
	case 0xAA62: return "_SetMCInfo";
//	case 0xAA63: return "_DispMCInfo";
	case 0xAA63: return "_DisposeMCInfo";
	case 0xAA64: return "_GetMCEntry";
	case 0xAA65: return "_SetMCEntries";

/*

    ; Menu Manager

*/

	case 0xAA66: return "_MenuChoice";

/*

    ; Dialog Manager?

*/

	case 0xAA67: return "_ModalDialogMenuSetup";
	case 0xAA68: return "_DialogDispatch";

/*

    ; Font Manager

*/

	case 0xA814: return "_SetFractEnable";
	case 0xA854: return "_FontDispatch";

/*

    ; Palette Manager

*/

	case 0xAA90: return "_InitPalettes";
	case 0xAA91: return "_NewPalette";
	case 0xAA92: return "_GetNewPalette";
	case 0xAA93: return "_DisposePalette";
	case 0xAA94: return "_ActivatePalette";
	case 0xAA95: return "_SetPalette";
//	case 0xAA95: return "_NSetPalette";
	case 0xAA96: return "_GetPalette";
	case 0xAA97: return "_PmForeColor";
	case 0xAA98: return "_PmBackColor";
	case 0xAA99: return "_AnimateEntry";
	case 0xAA9A: return "_AnimatePalette";
	case 0xAA9B: return "_GetEntryColor";
	case 0xAA9C: return "_SetEntryColor";
	case 0xAA9D: return "_GetEntryUsage";
	case 0xAA9E: return "_SetEntryUsage";
	case 0xAA9F: return "_CTab2Palette";
	case 0xAAA0: return "_Palette2CTab";
	case 0xAAA1: return "_CopyPalette";
	case 0xAAA2: return "_PaletteDispatch";

/*

    ; Sound Manager

*/

	case 0xA800: return "_SoundDispatch";
	case 0xA801: return "_SndDisposeChannel";
	case 0xA802: return "_SndAddModifier";
	case 0xA803: return "_SndDoCommand";
	case 0xA804: return "_SndDoImmediate";
	case 0xA805: return "_SndPlay";
	case 0xA806: return "_SndControl";
	case 0xA807: return "_SndNewChannel";
	case 0xA06E: return "_SlotManager";
	case 0xA8B5: return "_ScriptUtil";
	case 0xA089: return "_SCSIAtomic";
	case 0xA815: return "_SCSIDispatch";
	case 0xA83F: return "_Long2Fix";
	case 0xA840: return "_Fix2Long";
	case 0xA841: return "_Fix2Frac";
	case 0xA842: return "_Frac2Fix";
	case 0xA843: return "_Fix2X";
	case 0xA844: return "_X2Fix";
	case 0xA845: return "_Frac2X";
	case 0xA846: return "_X2Frac";
	case 0xA847: return "_FracCos";
	case 0xA848: return "_FracSin";
	case 0xA849: return "_FracSqrt";
	case 0xA84A: return "_FracMul";
	case 0xA84B: return "_FracDiv";
	case 0xA84D: return "_FixDiv";
	case 0xA05E: return "_NMInstall";
	case 0xA05F: return "_NMRemove";

/*

    ; All QDOffscreen Routines go through one trap with a selector

*/

	case 0xAB1D: return "_QDExtensions";

/*

    ; UserDelay

*/

	case 0xA84C: return "_UserDelay";

/*

    ; Component Manager

*/

	case 0xA82A: return "_ComponentDispatch";


/*

	; PowerPC Mac

*/

	case 0xAAFE: return "_MixedModeMagic";
	case 0xAA59: return "_MixedModeDispatch";
	case 0xAA5A: return "_CodeFragmentDispatch";

/*

    ; Translation Manager

*/

	case 0xABFC: return "_TranslationDispatch";


/*
	; Human Interface Utilities
*/

	case 0xAADD: return "_HumanInterfaceUtilsDispatch";


//	case 0xA89F: return "_InitDogCow";
//	case 0xA89F: return "_EnableDogCow";
//	case 0xA89F: return "_DisableDogCow";
//	case 0xA89F: return "_Moof";
//	case 0xAA52: return "_HFSPinaforeDispatch";

    default: return NULL;
    }
}
