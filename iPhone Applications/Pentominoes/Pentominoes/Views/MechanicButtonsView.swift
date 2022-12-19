//
//  MechanicButtonsView.swift
//  Pentominoes
//
//  Created by user224354 on 9/15/22.
//

import SwiftUI

struct MechanicButtonsView: View {

    var body: some View {
        ZStack {
            MechanicButtonView(id: 6, mechanicName: "Reset")
            MechanicButtonView(id: 7, mechanicName: "Solve")
        }
    }
}

struct MechanicButtonsView_Previews: PreviewProvider {
    static var previews: some View {
        MechanicButtonsView()
    }
}
