//
//  SingleCharacterView.swift
//  BeeQ
//
//  Created by user224354 on 8/25/22.
//

import SwiftUI

struct SingleCharacterView: View {
    @EnvironmentObject var engine: BeeQEngine
    var index: Int
    var charValue: Character
    var background: CharacterModel = CharacterModel(sides: OneGame.characterOptions)
    var body: some View {
        Button(action: {
            engine.charPressed(char: charValue)
        }) {
            Text(String(charValue))
        }
        .frame(width: 100, height: 100)
        .background(charValue == engine.currentGame.mustContains ? background.colors[0] : background.colors[1], in: background)
        .offset(x: background.calculateOffset(shapeID: index, offsetType: 0), y: background.calculateOffset(shapeID: index, offsetType: 1))
        .accentColor(.orange)
        .tint(Color.black)
    }
}

struct SingleCharacterView_Previews: PreviewProvider {
    static var previews: some View {
        SingleCharacterView(index: 0, charValue: "A")
    }
}
