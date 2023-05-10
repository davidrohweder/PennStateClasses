//
//  StartCharacterView.swift
//  BeeQ
//
//  Created by user224354 on 9/8/22.
//

import SwiftUI

struct StartCharacterView: View {
    @EnvironmentObject var engine: BeeQEngine
    @State var count: Int
    var body: some View {
                Section (header: Text("Length of \(count)")) {
                    ForEach(engine.currentGame.availableCharacters, id: \.self) {char in engine.currentGame.totalPossibleWordFromChar(char: char, length: count) != 0 ? Text("Character \(String(char)) has \(engine.currentGame.totalPossibleWordFromChar(char: char, length: count))") : nil}
        }
    }
}

struct StartCharacterView_Previews: PreviewProvider {
    static var previews: some View {
        StartCharacterView(count: 5)
    }
}
