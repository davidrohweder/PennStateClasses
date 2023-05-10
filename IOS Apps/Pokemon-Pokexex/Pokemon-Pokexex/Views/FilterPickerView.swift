//
//  FilterPickerView.swift
//  Pokedex
//
//  Created by David Rohweder on 10/27/22.
//

import SwiftUI

struct FilterPickerView: View {
    @EnvironmentObject var manager : PokemonManager
    @Environment(\.dismiss) private var dismiss
    @AppStorage("Constants.Preferences.sectioning") var sectioning : SectionTypes = .none
    @AppStorage("Constants.Preferences.filterBy") var filterBy : PokemonType = .bug
    
    var body: some View {
        NavigationStack {
            Form {
                Section(header: Text("Filter Cards")) {
                    Picker("Options", selection: $sectioning) {
                        ForEach(SectionTypes.allCases) {s in
                            Text(s.rawValue).tag(s)
                        }
                    }
                }
                if sectioning == .specific {
                    Section(header: Text("Available Types")) {
                        Picker("Type", selection: $filterBy) {
                            ForEach(PokemonType.allCases) {s in
                                Text(s.rawValue).tag(s)
                            }
                        }.pickerStyle(MenuPickerStyle())
                    }
                }
            }
            .toolbar {
                ToolbarItem(placement:.navigationBarTrailing) {
                    Button("Done") {dismiss()}
                }
            }
        }
    }
}

struct FilterPickerView_Previews: PreviewProvider {
    static var previews: some View {
        FilterPickerView()
    }
}
