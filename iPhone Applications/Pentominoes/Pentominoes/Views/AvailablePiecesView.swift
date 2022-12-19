//
//  AvailablePiecesView.swift
//  Pentominoes
//
//  Created by user224354 on 9/15/22.
//

import SwiftUI

struct AvailablePiecesView: View {
    @EnvironmentObject var manager: PentominoManager
    var body: some View {
        ForEach ($manager.pieces) { $cPiece in
            SinglePieceView(piece: $cPiece)
        } 
    }
}

struct AvailablePiecesView_Previews: PreviewProvider {
    static var previews: some View {
        AvailablePiecesView()
    }
}
