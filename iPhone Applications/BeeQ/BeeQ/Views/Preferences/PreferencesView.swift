//
//  PreferencesView.swift
//  BeeQ
//
//  Created by user224354 on 8/25/22.
//

import SwiftUI

struct PreferencesView: View {
    @EnvironmentObject var engine: BeeQEngine
    var body: some View {
        VStack {
            Form {
                PreferenceForm(index: 0)
                PreferenceForm(index: 1)
            }
            DismissSheetView()
        }
    }
}

struct PreferencesView_Previews: PreviewProvider {
    static var previews: some View {
        PreferencesView()
    }
}
