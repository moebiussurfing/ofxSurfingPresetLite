#pragma once
#include "ofMain.h"

//--

#include "ofxSurfingPresetsLite.h"

#include "ofxSurfingImGui.h"

//----

class ofxSurfingPresetsLiteImGui : public ofxSurfingPresetsLite {
private:
	ofxSurfingGui * ui = nullptr;

public:
	void setUiPtr(ofxSurfingGui * _ui) {
		ui = _ui;
	}

	//----

	// 1. Main window
	// using different layouts.
public:
	void drawImGui(bool bWindowed = false, bool bMinimized_ = false, bool bFoldered_ = false, bool bOpen = true) {
		if (!bGui) return;

		bool bOpen_ = true;

		if (bWindowed) {
			IMGUI_SUGAR__WINDOWS_CONSTRAINTSW_SMALL_LOCKED_RESIZE;
			bOpen_ = ui->BeginWindow(bGui);
		}

		else if (bFoldered_) {
			bOpen_ = true;
		}

		if (bOpen_) {
			string s = filename;
			string sn = "Presets";
			//string sn = "PRESETS";

			bool b;
			if (!bFoldered_) {
				b = true;
				if (!bWindowed) ui->AddLabelBig(sn);
			} else {
				b = ui->BeginTree(sn, bOpen);
				if (b) ui->AddSpacing();
			}

			if (b) {
				if (bMinimized_) ui->Add(ui->bMinimize, OFX_IM_TOGGLE_ROUNDED);

				if (!ui->bMinimize) {
					ui->Add(bGuiExpand, OFX_IM_TOGGLE_ROUNDED_MINI);
					ui->AddSpacing();
				}

				// expanded
				if (bGuiExpand) {
					// maximized
					if (!ui->bMinimize) {
						ui->Add(vLoad, OFX_IM_BUTTON_SMALL, 2, true);
						ui->AddTooltip("Discard changes\nand reload file");
						//ui->Add(vSave, OFX_IM_BUTTON_SMALL_BORDER_BLINK, 2);
						if (ui->Add(vSave,
								(bAutoSave ? OFX_IM_BUTTON_SMALL : OFX_IM_BUTTON_SMALL_BORDER_BLINK), 2)) {
							bOverInputText = false;
							indexName = s;
						}
						ui->AddTooltip("Save changes\nto file");

						if (!bOverInputText) {
							ui->Add(vNew, OFX_IM_BUTTON_SMALL, 2);
							string s_;
							if (bKeyModCtrl) {
								s_ = "Release control\nto rename";
							} else {
								s_ = "Press control\nto create";
							}
							ui->AddTooltip(s_);
						} else {
							if (ui->AddButton("Cancel", OFX_IM_BUTTON_SMALL_BORDER_BLINK, 2)) {
								bOverInputText = false;
								bDoingNew = false;
							}
						}
						ui->SameLine();
						ui->Add(vDelete, OFX_IM_BUTTON_SMALL, 2);
						ui->AddTooltip("Remove preset file");

						ui->Add(vRename, OFX_IM_BUTTON_SMALL, 2, true);
						ui->AddTooltip("Change file name");
						ui->Add(bAutoSave, OFX_IM_TOGGLE_SMALL_BORDER_BLINK, 2);
						string s_;
						if (bAutoSave) {
							s_ = "Edit Mode";
						} else {
							s_ = "Manual save Mode";
						}
						ui->AddTooltip(s_);
						ui->Add(vReset, OFX_IM_BUTTON_SMALL, 2, true);
						ui->AddTooltip("Preset reset");
						ui->Add(vRandom, OFX_IM_BUTTON_SMALL, 2);
						ui->AddTooltip("Preset random");

						//--

						ui->refreshLayout();
						float _w2 = getWidgetsWidth(2);
						float _h = getWidgetsHeightUnit();

						ui->AddLabel("KIT");

						if (ImGui::Button("Clear", ImVec2(_w2, _h))) {
							ImGui::OpenPopup("CLEAR KIT ?");
						}
						ui->AddTooltip("Remove all \nfile Presets!");

						if (ImGui::BeginPopupModal("CLEAR KIT ?", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
							ui->AddLabelBig("Presets kit \nwill be erased.", true, true);
							ui->AddSpacing();
							ui->AddLabelBig("This operation \ncannot be undone!", true, true);
							ui->AddSpacingBig();

							static bool dont_ask_me_next_time = false;
							ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
							ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
							ImGui::PopStyleVar();

							ui->AddSpacing();

							if (!dont_ask_me_next_time) {
								if (ImGui::Button("OK", ImVec2(120, 2 * _h))) {
									ofLogNotice("ofxSurfingPresetsLiteImGui") << "CLEAR";
									doClearKit();
									ImGui::CloseCurrentPopup();
								}
								ImGui::SetItemDefaultFocus();
								ImGui::SameLine();
								if (ImGui::Button("Cancel", ImVec2(120, 2 * _h))) {
									ImGui::CloseCurrentPopup();
								}
							} else {
								ofLogNotice("ofxSurfingPresetsLiteImGui") << "CLEAR";
								doClearKit(false);
								ImGui::CloseCurrentPopup();

								if (bOverInputText) bOverInputText = false;
							}

							ImGui::EndPopup();
						}

						ui->SameLine();
						ui->Add(vScanKit, OFX_IM_BUTTON_SMALL, 2);
						ui->AddTooltip("Reload folder files");

						if (ui->Add(vPopulateKit, OFX_IM_BUTTON_SMALL, 2)) {
							if (bOverInputText) bOverInputText = false;
						}
						ui->AddTooltip("Clear kit and create \nNew presets copying current");
						ui->SameLine();
						ui->Add(vPopulateRandomKit, OFX_IM_BUTTON_SMALL, 2);
						ui->AddTooltip("Clear kit and create \nNew randomized presets ");
					}

					// minimized
					else {
						if (ui->Add(vSave,
								(bOverInputText ? OFX_IM_BUTTON_SMALL_BORDER_BLINK : OFX_IM_BUTTON_SMALL), 2, true)) {
							bOverInputText = false;
							indexName = s;
						}

						if (!bOverInputText) {
							ui->Add(vNew, OFX_IM_BUTTON_SMALL, 2);
						} else {
							if (ui->AddButton("Cancel", OFX_IM_BUTTON_SMALL_BORDER_BLINK, 2)) {
								bOverInputText = false;
								bDoingNew = false;
							}
						}
					}

					//--

					if (bOverInputText) {
						if (!ui->bMinimize) ui->AddSpacing();

						int _w = ui->getWidgetsWidth() * 0.9f;
						ImGui::PushItemWidth(_w);
						{
							bool b = ImGui::InputText("##NAME", &s);
							if (b) {
								ofLogNotice("ofxSurfingPresetsLiteImGui") << "InputText:" << s.c_str();
								setFilename(s);
							}
							if (ImGui::IsItemDeactivated() && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
								bOverInputText = false;
						}
						ImGui::PopItemWidth();
					}
				}

				if (ui->bMinimize && !bGuiExpand) {
					ui->Add(vPrevious, OFX_IM_BUTTON_MEDIUM, 2, true);
					ui->Add(vNext, OFX_IM_BUTTON_MEDIUM, 2);
				}

				//--

				if (!ui->bMinimize && index.getMax() > -1) ui->AddSpacingSeparated();

				if (bGuiExpand) // expanded
				{
					ui->AddCombo(index, filenames);
				} else // not expanded
				{
					if (!ui->bMinimize) {
						ui->Add(vPrevious, OFX_IM_BUTTON_MEDIUM, 2, true);
						ui->Add(vNext, OFX_IM_BUTTON_MEDIUM, 2);
						ui->AddCombo(index, filenames);
						ui->Add(vLoad, OFX_IM_BUTTON_SMALL, 2, true);
						ui->Add(vSave, (bAutoSave ? OFX_IM_BUTTON_SMALL : OFX_IM_BUTTON_SMALL_BORDER_BLINK), 2);
					}
				}

				//--

				if (!ui->bMinimize) ui->AddSpacingSeparated();

				//--

				drawImGuiClicker();
				// drawImGuiClicker(bWindowed,ui->bMinimize);

				//--

				if (b && bFoldered_) ui->EndTree();
			}
		}

		if (bWindowed && bOpen_) {
			ui->EndWindow();
		}
	}

	//----

	// 2. Presets Clicker

	void drawImGuiClicker(bool bWindowed = false, bool bMinimal = false) {
		bool bOpen_ = false;
		if (bWindowed) {
			bOpen_ = ui->BeginWindow(bGui);
			if (!bOpen_) {
				ui->EndWindow();
				return;
			}
		}

		//--

		if (!bMinimal) {
			if (!ui->bMinimize) {
				ui->Add(bGuiClicker, OFX_IM_TOGGLE_BUTTON_ROUNDED_MINI);
				ui->AddSpacing();
			}
		} else {
			if (!bGuiClicker) bGuiClicker = true;
		}

		if (bGuiClicker) {
			if (ui->bMinimize && !bMinimal) ui->AddSpacingSeparated();

			float _h2 = ui->getWidgetsHeightUnit();
			_h2 *= 1.5f;

			bool bResponsiveButtonsClicker = true;

			//TODO: add a public var
			bool bFlip = false;

			string toolTip = "";
			if (bKeyModCtrl)
				toolTip = "Copy To";
			else if (bKeyModAlt)
				toolTip = "Swap With";

			if (bGuiColorized) {
				ofxImGuiSurfing::AddMatrixClickerLabels(index, keyCommandsChars, colors, bResponsiveButtonsClicker,
					guiRowAmount, true, _h2, toolTip, bFlip);
			} else {
				ofxImGuiSurfing::AddMatrixClickerLabels(index, keyCommandsChars, bResponsiveButtonsClicker,
					guiRowAmount, true, _h2, toolTip, bFlip);
			}
			if (bGuiExpand) {
				if (dir.size() > 0) {
					if (!bMinimal && !ui->bMinimize) {
						ui->AddSpacing();
						if (!ui->bMinimize) {
							ui->Add(guiRowAmount, OFX_IM_STEPPER, 2);
							ui->AddTooltip("Buttons per row.");
						}
					}
				}
			}
		}

		//--

		if (bWindowed) {
			if (bOpen_) {
				ui->EndWindow();
			}
		}
	}

	//----

	// 3. Only draw UI

	void drawGui() {
		if (!bGui) return;
		drawHelp();

		if (ui == nullptr) return;
		ui->Begin();
		{
			string n;
			if (bGuiFloating)
				n = "PARAMETERS " + name;
			else
				n = "PRESETS " + name;
			//n = ofToUpper(n);

			if (ui->BeginWindow(n)) {
				ui->Add(ui->bMinimize, OFX_IM_TOGGLE_ROUNDED_SMALL);
				ui->Add(bGuiFloating, OFX_IM_TOGGLE_ROUNDED_MINI);
				ui->AddSpacingSeparated();

				if (!bGuiFloating)
					ui->AddLabelBig("Parameters");
				ui->AddSpacing();

				ui->AddGroup(paramsPreset);

				// A.
				// Integrated inside another panel
				if (!bGuiFloating) {
					ui->AddSpacingBigSeparated();

					drawImGui();
				}

				// B.
				// A minimal clicker only version
				// drawImGuiClicker();

				ui->EndWindow();
			};

			// C.
			// Separated on another floating window
			if (bGuiFloating) drawImGui(true);
		}
		ui->End();
	}
};
