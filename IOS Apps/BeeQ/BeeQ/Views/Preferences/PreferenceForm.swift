//
//  PreferenceForm.swift
//  BeeQ
//
//  Created by user224354 on 9/6/22.
//

import SwiftUI

struct PreferenceForm: View {
    @EnvironmentObject var engine: BeeQEngine
    var index: Int
    var body: some View {
            Section (header: Text(engine.sessionPreferences.preferences[index].title)) {
                Picker("locale", selection: $engine.sessionPreferences.preferences[index].picked) {
                    ForEach(engine.sessionPreferences.preferences[index].options.sorted(by: <), id: \.key) { key, value in Text(value).tag(key)}
                }
                .onChange(of: engine.sessionPreferences.preferences[index].picked, perform: {
                    _ in engine.restartGame()
                })
                .pickerStyle(.segmented)
            }
    }
}

struct PreferenceForm_Previews: PreviewProvider {
    static var previews: some View {
        //PreferenceForm(preference: .constant(PForm(uid: 1, titleString: "test", formOptions: [0:"1"])))
        PreferenceForm(index: 0)
    }
}
