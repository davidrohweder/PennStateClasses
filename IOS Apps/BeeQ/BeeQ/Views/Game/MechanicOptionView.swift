//
//  MechanicOptionView.swift
//  BeeQ
//
//  Created by user224354 on 8/25/22.
//

import SwiftUI

struct MechanicOptionView: View {
    @EnvironmentObject var engine: BeeQEngine
    var body: some View {
        HStack {
            ForEach(engine.gameMechanics.buttons, id: \.self) {button in MechanicButtonView(id: button.id, sfname: button.ico, enabled: button.disabled)}
        }
    }
}

struct MechanicOptionView_Previews: PreviewProvider {
    static var previews: some View {
        MechanicOptionView()
    }
}
