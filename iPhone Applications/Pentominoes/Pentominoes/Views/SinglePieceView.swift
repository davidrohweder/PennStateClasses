//
//  SinglePieceView.swift
//  Pentominoes
//
//  Created by user224354 on 9/15/22.
//

import SwiftUI

struct SinglePieceView: View {
    @EnvironmentObject var manager: PentominoManager
    @Binding var piece: Piece
    @State var offset = CGSize.zero
    @State var scale: CGFloat = 1.0
    
    var body: some View {
        
        let movePiece = DragGesture()
            .onChanged { value in
                dragChanged(v: value)
                scale = 1.2
            }
            .onEnded { value in
                  dragEnded(v: value)
            }
        let rotatePiece = TapGesture()
            .onEnded {
                shortPressed()
            }
        let flipPieceOnY = LongPressGesture(minimumDuration: 2.0)
            .onEnded { _ in
                longPressed()
            }
        
        return Image(piece.tile.name)
            .rotationEffect(.degrees(CGFloat(90 * piece.position.rotations)))
            .rotation3DEffect(.degrees(piece.position.isFlipped ? 180 : 0), axis: (x: 0, y: 1, z: 0))
            .position(CGPoint(x: piece.centerPosition.0, y: piece.centerPosition.1))
            .offset(offset)
            .gesture(movePiece)
            .gesture(rotatePiece)
            .gesture(flipPieceOnY)
            .scaleEffect(scale)
            .animation(Animation.linear(duration: 0.5))
    }

    func dragChanged(v: DragGesture.Value) {
        self.offset = v.translation
    }

    func dragEnded(v: DragGesture.Value) {
        // arith to 40 block
        // update pos to offset
        piece.position.x += Int(v.translation.width / 40.0)
        piece.position.y += Int(v.translation.height / 40.0)
        self.scale = 1.0
        self.offset = CGSize.zero
    }
    
    func shortPressed() {
        self.piece.position.rotations = self.piece.position.rotations == 3 ? 0 : self.piece.position.rotations + 1
    }
    
    func longPressed() {
        piece.position.isFlipped.toggle()
    }
    
}

struct SinglePieceView_Previews: PreviewProvider {
    static var previews: some View {
        SinglePieceView(piece: .constant(Piece(uid: 0, X: 0, Y: 0, initTile: Tile(name: "F", width: 2, height: 3))))
    }
}
