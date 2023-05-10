//
//  CharacterOptionView.swift
//  BeeQ
//
//  Created by user224354 on 8/25/22.
//

import SwiftUI

struct CharacterOptionView: View {
    @EnvironmentObject var engine: BeeQEngine
    @State var idx: Int = 0
    var body: some View {
        ZStack {
            ForEach(engine.currentGame.availableCharacters, id: \.self) {char in
                SingleCharacterView(index: engine.currentGame.availableCharacters.firstIndex(of: char) ?? 0, charValue: char)
                
            }
        }.frame(height: 400)
    }
}


struct CharacterOptionView_Previews: PreviewProvider {
    static var previews: some View {
        CharacterOptionView()
    }
}
