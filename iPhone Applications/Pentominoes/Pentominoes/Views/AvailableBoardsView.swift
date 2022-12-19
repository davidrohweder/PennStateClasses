//
//  AvailableBoardsView.swift
//  Pentominoes
//
//  Created by user224354 on 9/15/22.
//

import SwiftUI

struct AvailableBoardsView: View {
    @EnvironmentObject var manager: PentominoManager
    var body: some View {
        ForEach (manager.boards) { board in
            BoardButtonView(buttonID: board.buttonID)
        }
    }
}

struct AvailableBoardsView_Previews: PreviewProvider {
    static var previews: some View {
        AvailableBoardsView()
    }
}
