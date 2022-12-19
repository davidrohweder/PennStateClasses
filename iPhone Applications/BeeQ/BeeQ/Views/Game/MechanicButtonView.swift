//
//  MechanicButtonView.swift
//  BeeQ
//
//  Created by user224354 on 8/30/22.
//

import SwiftUI

struct MechanicButtonView: View {
    @EnvironmentObject var engine: BeeQEngine
    var id: Int
    var sfname: String
    var enabled: Bool
    var body: some View {
        Button(action: { engine.actionCommand(command: id)}) {
            Image(systemName: sfname)
        }
        .buttonStyle(.borderedProminent)
        .foregroundColor(Color.orange)
        .tint(Color.black)
        .disabled(enabled)
    }
}

struct MechanicButtonView_Previews: PreviewProvider {
    static var previews: some View {
        MechanicButtonView(id: 1, sfname: "delete.left.fill", enabled: true)
    }
}
