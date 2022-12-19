//
//  MechanicButtonView.swift
//  Pentominoes
//
//  Created by user224354 on 9/18/22.
//

import SwiftUI

struct MechanicButtonView: View {
    @EnvironmentObject var manager: PentominoManager
    var id: Int
    var mechanicName: String
    var centerX: CGFloat = UIScreen.main.bounds.width / 2
    var centerY: CGFloat = UIScreen.main.bounds.height / 2 - 250
    var body: some View {
        Button(action: { manager.MechanicFunctions(id: id) }) {
            Text(mechanicName)
                .tint(.red)
        }
        .disabled(id == 7 && manager.currentBoard == 0 ? true : false)
        .position(x: id % 2 == 0 ? centerX - (Board.boardSize / 2) - 115 : centerX + (Board.boardSize / 2) + 65, y: centerY - (Board.boardSize / 2) + CGFloat(Int(id / 2) * 125))
    }
}

struct MechanicButtonView_Previews: PreviewProvider {
    static var previews: some View {
        MechanicButtonView(id: 0, mechanicName: "Reset")
    }
}
