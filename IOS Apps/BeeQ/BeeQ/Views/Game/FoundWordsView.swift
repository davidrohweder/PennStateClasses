//
//  FoundWordsView.swift
//  BeeQ
//
//  Created by user224354 on 8/25/22.
//

import SwiftUI

struct FoundWordsView: View {
    @EnvironmentObject var engine: BeeQEngine
    var body: some View {
        ScrollView (.horizontal) {
            HStack {
                ForEach(engine.currentSession.foundWords, id: \.self) {char in Text(" \(char). ")}
            }
        }
        .padding()
        .frame(maxWidth: .infinity)
        .background(Color.black)
        .foregroundColor(Color.white)
    }
}

struct FoundWordsView_Previews: PreviewProvider {
    static var previews: some View {
        FoundWordsView()
    }
}
