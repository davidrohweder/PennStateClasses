//
//  MainBoardView.swift
//  Pentominoes
//
//  Created by user224354 on 9/15/22.
//

import SwiftUI

struct MainBoardView: View {
    @EnvironmentObject var manager: PentominoManager
    var body: some View {
        Image(manager.boards[manager.currentBoard].boardName)
            .frame(width: Board.boardSize, height: Board.boardSize)
            .position(x: CGFloat(Int(UIScreen.main.bounds.width / 2 / 40) * 40), y: CGFloat(Int(((UIScreen.main.bounds.height / 2 - 350) / 40)) * 40))
    }
}

struct MainBoardView_Previews: PreviewProvider {
    static var previews: some View {
        MainBoardView()
    }
}
