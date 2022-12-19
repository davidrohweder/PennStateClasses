//
//  BoardOptionView.swift
//  Pentominoes
//
//  Created by user224354 on 9/15/22.
//

import SwiftUI

struct BoardButtonView: View {
    @EnvironmentObject var manager: PentominoManager
    var buttonID: Int
    var body: some View {
        Button(action: { manager.boardSwitch(id: buttonID) }) {
            Image(manager.boards[buttonID].boardButtonName)
        }
        .position(x: manager.boards[buttonID].buttonLocationX, y: manager.boards[buttonID].buttonLocationY)
    }
}

struct BoardOptionView_Previews: PreviewProvider {
    static var previews: some View {
        BoardButtonView(buttonID: 1)
    }
}
