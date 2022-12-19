//
//  TypedCharacterView.swift
//  BeeQ
//
//  Created by user224354 on 8/25/22.
//

import SwiftUI

struct TypedCharacterView: View {
    @EnvironmentObject var engine: BeeQEngine
    var body: some View {
        VStack {
            Divider()
            HStack (spacing: 5) {
                ForEach (0..<engine.currentGame.currentWord.count, id: \.self) {index in
                    let char = String(engine.currentGame.currentWord[engine.currentGame.currentWord.index(engine.currentGame.currentWord.startIndex, offsetBy: index)])
                    if String(engine.currentGame.mustContains) == char {
                        Text(char).foregroundColor(.yellow)
                    } else {
                        Text(char).foregroundColor(.black)
                    }
                }
                
            }
            Divider()
        }
    }
}

struct TypedCharacterView_Previews: PreviewProvider {
    static var previews: some View {
        TypedCharacterView()
    }
}
